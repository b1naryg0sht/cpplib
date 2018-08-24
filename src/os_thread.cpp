#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "os_thread.h"

namespace Yogurt {


void* thread_proc(void* arg)
{
    OSThread* thread = (OSThread*)arg;
	if(thread)
	{
		int nRet = 0;

		nRet = thread->prepare();
		if(nRet != 0)
		{
			printf("prepare thread falied, ret:%d\n", nRet);
			printf("--- thread(thr:0x%lx) is over ---\n", OSThread::thread_id());
			return NULL;
		}

		
		while(true)
		{
			
	    	nRet = thread->svc();
			if(nRet != 0)
			{
				printf("thread svr failed, ret:%d\n", nRet);
				break;	
			}

			
			if(thread->is_stop())
			{
				printf("thread(thr:0x%lx) is stop.\n", OSThread::thread_id());
				break;			
			}
			
		}

		
		thread->cleanup();	
		thread->thr_status(OSThread::ST_EXIT);
		
	}
	
    return NULL;
	
}



OSThread::OSThread(): _thr_cnt(1), _status(ST_EXIT), _stop(false), \
	_detach(true), _stack_size(0)
{
	_threads.clear();
}


OSThread::~OSThread()
{
}



int OSThread::do_init(void *args)
{
	return 0;
}



int OSThread::init(void *args, unsigned int thr_cnt, bool detach, unsigned int stack_size)
{
	int nRet = 0;

	if(_status == ST_INIT)
	{
		printf("the thread is init ago.\n");
		return 0;		
	}

	_stop = false;
	_detach = detach;
	_stack_size = stack_size;
	_thr_cnt = thr_cnt;
	
    nRet = pthread_attr_init(&_attr);
    if (nRet != 0)
    {
        printf("pthread_attr_init failed. ret:%d, errno:%d, errmsg:%s\n", 
				nRet, errno, strerror(errno));
		return nRet;
    }


    if (stack_size > 0)
    {
		nRet = pthread_attr_setstacksize(&_attr, stack_size);
		if (nRet != 0)
	    {
	        printf("pthread_attr_setstacksize failed. ret:%d, errno:%d, errmsg:%s\n", 
					nRet, errno, strerror(errno));
			return nRet;
	    }
			
	}

	nRet = do_init(args);
	if (nRet != 0)
	{
		printf("do_init failed. ret:%d\n", nRet);
		return nRet;
	}
	
	_status = ST_INIT;
	
	return nRet;
	
}




int OSThread::run(void **ret)
{
	int nRet = 0;

	if(_status != ST_INIT)
	{
		printf("the status of thread is invalid. status:%d\n", _status);
		return -2;		
	}

	 pthread_t thread = 0;
	for(unsigned int i=0; i<_thr_cnt; i++)
	{
		nRet = pthread_create(&thread , &_attr, thread_proc, this);
		if(nRet != 0)
		{
			printf("pthread_create failed. ret:%d, errno:%d, errmsg:%s\n", 
					nRet, errno, strerror(errno));
		}
		else
		{
			printf("--- pthread_create(thr:0x%lx) success ---\n", thread);
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


int OSThread::exit(void *ret)
{
	int nRet = 0;

	pthread_exit(ret);

	_status = ST_EXIT;

	return nRet;
	
}



pthread_t OSThread::thread_id()
{
	return pthread_self();
}

void OSThread::stop()
{
	_stop = true;
}


bool OSThread::is_stop()
{
	return _stop;
}


int OSThread::join(pthread_t thread, void **ret)
{
	int nRet = 0;

    nRet = pthread_join(thread, ret);
	if (nRet != 0)
	{
		printf("pthread_join(thr:%u) failed. ret:%d, errno:%d, errmsg:%s\n", 
				(unsigned int)thread, nRet, errno, strerror(errno));
	}

	return nRet;
	
}




int OSThread::detach(pthread_t thread)
{
	int nRet = 0;
	
    nRet = pthread_detach(thread);
    if (nRet != 0)
    {
		printf("pthread_detach(thr:0x%lx) failed. ret:%d, errno:%d, errmsg:%s\n", 
				thread, nRet, errno, strerror(errno));
	}

	return nRet;
}


int OSThread::thr_count()
{
	return _threads.size();
}



void OSThread::thr_status(int status)
{
	_status =  status;
}

int OSThread::thr_status()
{
	return _status;
}

}// namespace Yogurt
