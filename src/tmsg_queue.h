#ifndef __YOGURT_TMSG_QUEUE_H__
#define __YOGURT_TMSG_QUEUE_H__

#include <pthread.h>
#include <vector>

namespace Yogurt {

template <typename T> 
class TMsgQueue {
private:
    std::vector<T> _queue;

    pthread_mutex_t _mutex;
    pthread_cond_t _cond;

public:

    TMsgQueue()
    {
        pthread_mutex_init(&_mutex, NULL);
        pthread_cond_init(&_cond, NULL);
    }
    
    ~TMsgQueue()
    {
        pthread_mutex_destroy(&_mutex);
        pthread_cond_destroy(&_cond);

        _queue.clear();
    }

    void push(T& data)
    {
        pthread_mutex_lock(&_mutex);
        _queue.push_back(data);
        pthread_cond_broadcast(&_cond);
        pthread_mutex_unlock(&_mutex);
    }

    int size()
    {
      return _queue.size();
    }

    T pop()
    {           	
        pthread_mutex_lock(&_mutex);

        while(_queue.empty() == true)
        {
            pthread_cond_wait(&_cond, &_mutex);
        }

        T p = _queue.front();
        _queue.erase(_queue.begin());
        pthread_mutex_unlock(&_mutex);
		return p;
    }

	void clear()
	{
        pthread_mutex_lock(&_mutex);
        _queue.clear();
        pthread_mutex_unlock(&_mutex);
	}
};

}// namespace Yogurt
#endif //__YOGURT_TMSG_QUEUE_H__