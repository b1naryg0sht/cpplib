#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/prctl.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include "Thread.h"
#include "Log.h"

namespace cppbrick {


void* thread_proc(void* arg)
{
    Thread* thread = (Thread*)arg;
    
    CB_LOG_DEBUG("--- thread create success tid:0x%lx, pid:%d, name:%s", 
    		pthread_self(), syscall(SYS_gettid), thread->thr_name().c_str());
	if(thread)
	{
		int nRet = 0;

		nRet = thread->prepare();
		if(nRet != 0)
		{
			CB_LOG_ERROR("prepare thread falied, ret:%d, thread(tid:0x%lx pid:%d name:%s) exit", 
				nRet, pthread_self(), syscall(SYS_gettid), thread->thr_name().c_str());
			return NULL;
		}

		
		while(true)
		{
			
	    	nRet = thread->svc();
			if(nRet != 0)
			{
				CB_LOG_ERROR("thread(tid:0x%lx pid:%d name:%s) svr failed, ret:%d", 
						pthread_self(), syscall(SYS_gettid), thread->thr_name().c_str(), nRet);
				break;	
			}

			
			if(thread->is_stop())
			{
				CB_LOG_DEBUG("thread(tid:0x%lx pid:%d name:%s) is stop.", 
					pthread_self(), syscall(SYS_gettid), thread->thr_name().c_str());
				break;			
			}
			
		}

		
		thread->clean_up();	
		thread->thr_status(Thread::ST_EXIT);
		
	}
	
    return NULL;
	
}



Thread::Thread(): _thr_cnt(1), _status(ST_EXIT), _stop(false), \
	_detach(true), _stack_size(0)
{
	_threads.clear();
}


Thread::~Thread()
{
}


int Thread::init(void *args, unsigned int thr_cnt, bool detach, unsigned int stack_size, std::string name)
{
	int nRet = 0;

	if(_status == ST_INIT)
	{
		CB_LOG_WARN("the thread is init ago.");
		return 0;		
	}

	_stop = false;
	_detach = detach;
	_stack_size = stack_size;
	_thr_cnt = thr_cnt;

	if(name != "")
	{
		prctl(PR_SET_NAME, name.c_str());
	}
	
    nRet = pthread_attr_init(&_attr);
    if (nRet != 0)
    {
        CB_LOG_ERROR("pthread_attr_init failed. ret:%d, errno:%d, errmsg:%s", nRet, errno, strerror(errno));
		return nRet;
    }


    if (stack_size > 0)
    {
		nRet = pthread_attr_setstacksize(&_attr, stack_size);
		if (nRet != 0)
	    {
	        CB_LOG_ERROR("pthread_attr_setstacksize failed. ret:%d, errno:%d, errmsg:%s", 
					nRet, errno, strerror(errno));
			return nRet;
	    }
			
	}

	nRet = do_init(args);
	if (nRet != 0)
	{
		CB_LOG_ERROR("do_init failed. ret:%d", nRet);
		return nRet;
	}
	
	_status = ST_INIT;
	
	return nRet;
	
}




int Thread::run(void **ret)
{
	int nRet = 0;

	if(_status != ST_INIT)
	{
		CB_LOG_ERROR("the status of thread is invalid. status:%d", _status);
		return -2;		
	}

	 pthread_t thread = 0;
	for(unsigned int i=0; i<_thr_cnt; i++)
	{
		nRet = pthread_create(&thread , &_attr, thread_proc, this);
		if(nRet != 0)
		{
			CB_LOG_ERROR("pthread_create failed. ret:%d, errno:%d, errmsg:%s", 
				nRet, errno, strerror(errno));
				
			continue;
		}

		_threads.push_back(thread);
		
		_status = ST_RUN;
		
		if(_detach)
		{
			nRet = detach(thread);
		}

	}
	

	if(!_detach)
	{
		nRet = join(thread, ret);
	}
		
	return nRet;
	
}


int Thread::exit(void *ret)
{
	int nRet = 0;

	pthread_exit(ret);

	_status = ST_EXIT;

	return nRet;
	
}



pthread_t Thread::thread_id()
{
	return pthread_self();
}

void Thread::stop()
{
	_stop = true;
}


bool Thread::is_stop()
{
	return _stop;
}


int Thread::join(pthread_t thread, void **ret)
{
	int nRet = 0;

    nRet = pthread_join(thread, ret);
	if (nRet != 0)
	{
		CB_LOG_ERROR("pthread_join(thr:%u) failed. ret:%d, errno:%d, errmsg:%s", 
				(unsigned int)thread, nRet, errno, strerror(errno));
	}

	return nRet;
	
}




int Thread::detach(pthread_t thread)
{
	int nRet = 0;
	
    nRet = pthread_detach(thread);
    if (nRet != 0)
    {
		CB_LOG_ERROR("pthread_detach(thr:0x%lx) failed. ret:%d, errno:%d, errmsg:%s", thread, nRet, errno, strerror(errno));
	}

	return nRet;
}


int Thread::thr_count()
{
	return _threads.size();
}



void Thread::thr_status(int status)
{
	_status =  status;
}

int Thread::thr_status()
{
	return _status;
}

std::string Thread::thr_name()
{
	return _name;
}

}// namespace cppbrick
