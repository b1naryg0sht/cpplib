#ifndef __CPPBRICK_TQUEUE_H__
#define __CPPBRICK_TQUEUE_H__

#include <ThreadMutex.h>
#include <ThreadCond.h>
#include <StdTypes.h>
#include <deque>
#include <TimeStamp.h>
#include <Log.h>

namespace cppbrick {


template <typename T>
class TQueue
{
private:
	std::deque<T> _lst_item;
	ThreadMutex _mutex;
	ThreadCond _cond;
	uint_t _max_size;  //队列最大限制, 0表示没有限制
	TimeStamp _wake_up_ts;  //线程睡醒时间
	uint_t _rate_limit;  //限速值，单位: 元素个数/每秒, 0表示不限速
	uint_t _try_pop_cnt;


public:
	TQueue(const uint_t max_size=0, const uint_t rate_limit=0)
	:_cond(_mutex), _max_size(max_size), _rate_limit(rate_limit), _try_pop_cnt(0)
	{}

	~TQueue() {}

	uint_t size()
	{
		ThreadMutexGuard mutex_guard(_mutex);
		return _lst_item.size();
	}

	bool empty()
	{
		ThreadMutexGuard mutex_guard(_mutex);
		return _lst_item.empty();
	}	

	bool full()
	{
		ThreadMutexGuard mutex_guard(_mutex);
		return (_max_size > 0) && (_lst_item.size() > _max_size);
	}

	int pop(T &item)
	{
		TimeStamp last_pop_ts;
		{
			ThreadMutexGuard mutex_guard(_mutex);
			
			while(_lst_item.empty() == true)
			{
				_cond.wait();
			}
			
			item = _lst_item.front();
			_lst_item.pop_front();
			TimeStamp pop_ts;
			
			CB_LOG_DEBUG("thread %lx at %ld:%ld consume %d\n", pthread_self(), pop_ts.getSeconds(), pop_ts.getUSeconds(), item);
			if(_rate_limit > 0)
			{		

				_try_pop_cnt++;
				if(_wake_up_ts < pop_ts)
				{
					TimeStamp delta_cond_wait = pop_ts - _wake_up_ts;
					if(delta_cond_wait.ToUSeconds()/1000000 > 1)
					{
						_try_pop_cnt = 1;
						_wake_up_ts.setSeconds(pop_ts.getMSeconds()-1);
						_wake_up_ts.setUSeconds(0);
					}
					else
					{
						if(_try_pop_cnt >= _rate_limit)
						{	
							_try_pop_cnt = 0;
							_wake_up_ts.setSeconds(pop_ts.getSeconds()+1);
							_wake_up_ts.setUSeconds(0);
							last_pop_ts = _wake_up_ts;
						}
					}
				}
				else
				{	
					last_pop_ts = pop_ts;
				}
			}
		}
		// no lock
		if(_rate_limit > 0 && _wake_up_ts > last_pop_ts)
		{
			TimeStamp howlong = _wake_up_ts - last_pop_ts;
			//CB_LOG_DEBUG("thread 0x%lx sleep %ld, wake up at %ld:%ld\n",
			//	pthread_self(), howlong.ToUSeconds(), _wake_up_ts.getSeconds(), _wake_up_ts.getUSeconds());

			usleep(howlong.ToUSeconds());
		}

		return 0;
	}

	int pop(T &item, const uint_t timeout)
	{
		TimeStamp last_pop_ts;
		{
			ThreadMutexGuard mutex_guard(_mutex);
		
			if(_lst_item.empty() == true)
			{
				_cond.TimeWait(timeout);
			}

			if(_lst_item.empty() == true)
			{
				return -1;
			}
		
			item = _lst_item.front();
			_lst_item.pop_front();
			TimeStamp pop_ts;
			
			CB_LOG_DEBUG("thread %lx at %ld:%ld consume %d\n", pthread_self(), pop_ts.getSeconds(), pop_ts.getUSeconds(), item);
			if(_rate_limit > 0)
			{		
		
				_try_pop_cnt++;
				if(_wake_up_ts < pop_ts)
				{
					TimeStamp delta_cond_wait = pop_ts - _wake_up_ts;
					if(delta_cond_wait.ToUSeconds()/1000000 > 1)
					{
						_try_pop_cnt = 1;
						_wake_up_ts.setSeconds(pop_ts.getMSeconds()-1);
						_wake_up_ts.setUSeconds(0);
					}
					else
					{
						if(_try_pop_cnt >= _rate_limit)
						{	
							_try_pop_cnt = 0;
							_wake_up_ts.setSeconds(pop_ts.getSeconds()+1);
							_wake_up_ts.setUSeconds(0);
							last_pop_ts = _wake_up_ts;
						}
					}
				}
				else
				{	
					last_pop_ts = pop_ts;
				}
			}
		}
		// no lock
		if(_rate_limit > 0 && _wake_up_ts > last_pop_ts)
		{
			TimeStamp howlong = _wake_up_ts - last_pop_ts;
			//CB_LOG_DEBUG("thread 0x%lx sleep %ld, wake up at %ld:%ld\n",
			//	pthread_self(), howlong.ToUSeconds(), _wake_up_ts.getSeconds(), _wake_up_ts.getUSeconds());
		
			usleep(howlong.ToUSeconds());
		}


		return 0;

	}

	int push(T &item)
	{
		ThreadMutexGuard mutex_guard(_mutex);
		//queue full
		if((_max_size > 0) && (_lst_item.size() > _max_size))
		{
			CB_LOG_WARN("reach max queue size.\n");
			return -1;
		}
		
		_lst_item.push_back(item);
		_cond.broadcast();

		return 0;
	}
};


}

#endif


