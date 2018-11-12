#ifndef __CPPBRICK_THREADMUTEX_H__
#define __CPPBRICK_THREADMUTEX_H__

#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include <cstdlib>


namespace cppbrick {
	

class ThreadMutex {
public:
	pthread_mutex_t mutex;

public:
	inline ThreadMutex() 
	{
		::pthread_mutex_init(&mutex, NULL);
	}

	inline void lock() 
	{
		::pthread_mutex_lock(&mutex);
	}

	inline void unlock() 
	{
		::pthread_mutex_unlock(&mutex);
	}

	inline ~ThreadMutex() 
	{
		::pthread_mutex_destroy(&mutex);
	}

private:
	ThreadMutex(const ThreadMutex& m);
	ThreadMutex& operator=(const ThreadMutex &m);
};

/**
 *	definition of ThreadMutexGuard;
 **/
class ThreadMutexGuard 
{
	private:
	ThreadMutex& _mutex;

	public:
	inline ThreadMutexGuard(ThreadMutex& mutex) :
		_mutex(mutex) 
	{
		_mutex.lock();
	}

	inline ~ThreadMutexGuard() 
	{
		_mutex.unlock();
	}
};

}

#endif