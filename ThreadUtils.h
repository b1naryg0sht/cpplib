#ifndef __TCPPLIB__THREAD_UTILS_H__
#define __TCPPLIB__THREAD_UTILS_H__

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
        class Mutex {
            private:
            pthread_mutex_t mutex;

            public:
            inline Mutex() {
                ::pthread_mutex_init(&mutex, NULL);
            }

            inline void lock() {
                ::pthread_mutex_lock(&mutex);
            }

            inline void unlock() {
                ::pthread_mutex_unlock(&mutex);
            }

            inline ~Mutex() {
                ::pthread_mutex_destroy(&mutex);
            }

            private:
            Mutex(const Mutex& m);
            Mutex& operator=(const Mutex &m);
        };

        /**
         *	definition of ScopedLock;
         **/
        class ScopedLock {
            private:
            Mutex& _mutex;

            public:
            inline ScopedLock(Mutex& mutex) :
                _mutex(mutex) {
                _mutex.lock();
            }

            inline ~ScopedLock() {
                _mutex.unlock();
            }
        };

}

#endif //__TCPPLIB__THREAD_UTILS_H__
