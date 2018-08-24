#ifndef __YOGURT__THREAD_UTILS_H__
#define __YOGURT__THREAD_UTILS_H__

#include <stdio.h>
#include <pthread.h>
#include <string>
#include <assert.h>
#include <cstdlib>


namespace Yogurt {

        /** 
         * returns the thread ID
         **/
        std::string GetThreadId();
        
        /**
         **/
        class ThreadMutex 
        {
            private:
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

#endif //__YOGURT__THREAD_UTILS_H__
