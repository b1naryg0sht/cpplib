#ifndef __CPPBRICK_TVECTOR_H__
#define __CPPBRICK_TVECTOR_H__

#include <vector>
#include "ThreadMutex.h"
#include "ThreadCond.h"
#include "StdTypes.h"
#include "TimeStamp.h"
#include "Log.h"

namespace cppbrick {

/*
*流控策略：在每秒钟内尽可能快的pop出元素，一旦达到上限，就让线程进入睡眠,直到下一秒才醒来
*/

template <typename T>
class TVector
{
private:
	std::vector<T> _lst_item;
	ThreadMutex _mutex;
	ThreadCond _cond;
	uint_t _max_size;  //队列最大限制, 0表示没有限制
	TimeStamp _wake_up_ts;  //线程睡醒时间
	uint_t _rate_limit;  //限速值，单位: 元素个数/每秒, 0表示不限速
	uint_t _try_pop_cnt;

public:
	TVector(const uint_t max_size=0, const uint_t rate_limit=0)
	:_cond(_mutex), _max_size(max_size), _rate_limit(rate_limit), _try_pop_cnt(0)
	{}

	~TVector() {}

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
			_lst_item.erase(_lst_item.begin());
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
			_lst_item.erase(_lst_item.begin());
			TimeStamp pop_ts;
			
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
			CB_LOG_WARN("reach max queue size.");
			return -1;
		}

		_lst_item.push_back(item);
		_cond.broadcast();

		return 0;
	}
};


}// namespace cppbrick
#endif
