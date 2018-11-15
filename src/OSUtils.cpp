#include <string>
#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <string.h>
#include <stdarg.h>
#include <signal.h>
#include "OSUtils.h"


namespace cppbrick {

std::string OSUtils::GetThreadId() 
{
	char buffer[4*sizeof(long)] = {0};
	int bufsize = sizeof(buffer)/sizeof(buffer[0]);
	int n = ::snprintf(buffer, bufsize, "0x%lx", pthread_self());	// thread id unsigned
	if (n >= bufsize) 
	{
		// escape to heap allocation
		char *buff_alloc;
		int size = ::asprintf(&buff_alloc, "0x%lx", pthread_self());	// thread id unsigned
		if (size < 0) 
		{
			throw std::bad_alloc();
		}
		std::string res(buff_alloc);
		free(buff_alloc);
		return res;
	}
	return std::string(buffer);
}

int OSUtils::get_rlimit(int resource, struct rlimit *rlim)
{
	int nRet = 0;
	
	nRet = getrlimit(resource, rlim);
	if(nRet != 0)
	{
		printf("getrlimit failed. resource:%d, errno:%d, errmsg:%s\n", 
			resource, errno, strerror(errno));
	}

	return nRet;

}

int OSUtils::signal_mask(int how, unsigned int num, ...)
{
	int nRet = 0;

	va_list args;
   	va_start(args, num);

	//初始化信号集
	sigset_t sigset;
	sigemptyset(&sigset);
	
	int sig = 0;
	for(unsigned int i=0; i<num; i++)
	{
		sig = va_arg(args, int); 
		nRet = sigaddset(&sigset, sig);
		if(nRet != 0)
		{
			printf("sigaddset failed. signo(%i):%d, errno:%d, errmsg:%s\n", 
				i, sig, errno, strerror(errno));
		}
	}
	va_end(args);
	
	nRet = pthread_sigmask(how, &sigset, NULL);
	if (nRet != 0)
	{
		printf("pthread_sigmask failed. ret:%d, errno:%d, errmsg:%s\n", 
				nRet, errno, strerror(errno));
	}
	
	return nRet;
	
}


}
