#include <time.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include "Log.h"
#include "ThreadCond.h"

namespace cppbrick {

ThreadCond::ThreadCond(ThreadMutex &mutex): _mutex(mutex)
{
	int nRet = 0;

	nRet = pthread_cond_init(&_cond, NULL);
	if(nRet != 0)
	{
		CB_LOG_ERROR("pthread_cond_init failed. ret:%d, errno:%d, errmsg:%s\n", nRet, errno, strerror(errno));
	}
}



ThreadCond::~ThreadCond()
{
	int nRet = 0;

	nRet = pthread_cond_destroy(&_cond);
	if(nRet == EBUSY)
	{
		CB_LOG_WARN("the ThreadCond is waited now, can't destroy, thread:0x%lx.\n",pthread_self()); 
			
	}
	else if(nRet < 0)
	{
		CB_LOG_ERROR("pthread_cond_destroy failed. ret:%d, errno:%d, errmsg:%s, thread:0x%lx\n", 
		nRet, errno, strerror(errno), pthread_self());
	}
	

}



int ThreadCond::wait()
{
	int nRet = 0;

	nRet = pthread_cond_wait(&_cond, &(_mutex.mutex));
	if(nRet != 0)
	{
		CB_LOG_ERROR("pthread_cond_wait failed. ret:%d, errno:%d, errmsg:%s, thread:0x%lx\n", nRet, errno, strerror(errno), pthread_self());
	}

	return nRet;

}


int ThreadCond::TimeWait(long usec)
{
	int nRet = 0;

	if(usec == 0)
	{
		nRet = wait();
		return nRet;
	}

	/*
	 struct timespec 
	 {
		  time_t   tv_sec;		  //seconds
		  long      tv_nsec; 	         //nanoseconds
	 };
	*/
	struct timespec ts;
	nRet = clock_gettime(CLOCK_REALTIME, &ts);
	if(nRet != 0)
	{
		CB_LOG_ERROR("clock_gettime failed. errno:%d, errmsg:%s\n", errno, strerror(errno));
		
		return nRet;
	}

	ts.tv_sec += usec / 1000000;
	ts.tv_nsec += (usec % 1000000) * 1000;

	//如果计算出来的纳秒数都超过1秒就进行进位处理
	if(ts.tv_nsec >= 1000000000)
	{
		ts.tv_sec += 1;
		ts.tv_nsec -= 1000000000;
	}
	
	nRet = pthread_cond_timedwait(&_cond, &(_mutex.mutex), &ts);
	if(nRet == 0)
	{
		//printf("pthread_cond_timedwait success.\n");
	}
	else if(nRet == ETIMEDOUT)
	{
		//printf("the ThreadCond is timeout!\n");
	}
	else
	{
		printf("pthread_cond_timedwait failed. ret:%d, errno:%d, errmsg:%s, thread:0x%lx\n", nRet, errno, strerror(errno), pthread_self());
	}

	return nRet;

}



int ThreadCond::signal()
{
	int nRet = 0;

	nRet = pthread_cond_signal(&_cond);
	if(nRet != 0)
	{
		CB_LOG_ERROR("pthread_cond_signal failed. ret:%d, errno:%d, errmsg:%s, thread:0x%lx\n", nRet, errno, strerror(errno), pthread_self());
	}
	
	return nRet;

}


int ThreadCond::broadcast()
{
	int nRet = 0;

	nRet = pthread_cond_broadcast(&_cond);
	if(nRet != 0)
	{
		CB_LOG_ERROR("pthread_cond_broadcast failed. ret:%d, errno:%d, errmsg:%s, thread:0x%lx\n", nRet, errno, strerror(errno), pthread_self());
	}
	
	return nRet;
}

}