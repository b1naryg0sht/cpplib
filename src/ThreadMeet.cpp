#include <stdio.h>
#include "ThreadMeet.h"

namespace cppbrick {

ThreadMeet::ThreadMeet(uint_t maxCount)
{
	pthread_mutexattr_t mutex_attr;
	pthread_condattr_t cond_attr;
 
	int status = 0;
	status |= pthread_mutexattr_init(&mutex_attr);
	status |= pthread_condattr_init(&cond_attr);  

	status |= pthread_mutex_init(&lock, &mutex_attr);
	status |= pthread_cond_init(&cond, &cond_attr);  

	if( maxCount <= 0 )
	{
		maxCount = 1;
	}

	count = maxCount = maxCount;

	if(status!=0)
	{
		printf("ThreadMeet init fails = %d \r\n", status);
	}
	
	pthread_condattr_destroy(&cond_attr);
	pthread_mutexattr_destroy(&mutex_attr);
}

/*
@timeout: > 0, only wait for a period since arrive R Point, in usec
		  = 0, wait all the time
*/
void ThreadMeet::Meet(uint_t timeout)
{
	pthread_mutex_lock(&lock);


	if(count > 0) 
	{
		count--;

		//all thread have get to R Point, then tell other thread to wake up
		if( count == 0)
		{
			pthread_cond_broadcast(&cond);
		}
		else
		{
			//wait for a certain period
			if( timeout > 0 )
			{
				time_t now = time(NULL);
				now += timeout/1000000;

				struct timespec ts;
				ts.tv_sec = now;
				ts.tv_nsec = ((timeout%1000000) * 1000);

				pthread_cond_timedwait(&cond, &lock, &ts);
			}
			//wait until all thread come to R Point
			else
			{
				pthread_cond_wait(&cond, &lock);
			}
			
		}

	}

	pthread_mutex_unlock(&lock);
}

void ThreadMeet::Reset()
{
	pthread_mutex_lock(&lock);

	count = maxCount;

	pthread_mutex_unlock(&lock);
}


void ThreadMeet::SetMaxcount(uint_t maxCount)
{
	pthread_mutex_lock(&lock);

	maxCount = maxCount;

	pthread_mutex_unlock(&lock);	
}

ThreadMeet::~ThreadMeet()
{
  pthread_cond_destroy(&cond);
  pthread_mutex_destroy(&lock); 
}

}
