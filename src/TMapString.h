#ifndef __CPPBRICK_TMAPSTRING_H__
#define __CPPBRICK_TMAPSTRING_H__

#include <map>
#include <ThreadMutex.h>
#include <ThreadCond.h>
#include <StdTypes.h>

namespace cppbrick {

template <typename T>
class TMapString
{
public:
	TMapString():_cond(_mutex)
	{
	}

	~TMapString()
	{
	}

	uint_t size()
	{
		ThreadMutexGuard guard(_mutex);
		return _items.size();
	}

	bool empty()
	{
		ThreadMutexGuard guard(_mutex);
		return _items.empty();

	}

	int pop(const std::string &key, T &item)
	{
		int nRet = 0;
		
		ThreadMutexGuard guard(_mutex);
#if 1
		// do we really need to wait here ?
		while(_items.empty())
		{
			nRet = _cond.wait();
			if(nRet != 0)
			{
				CB_LOG_ERROR("cond wait failed.\n");
				return -1;
			}
		}
#endif
		typename std::map<std::string, T >::iterator itr = _items.find(key);
		if(itr != _items.end())
		{
			item = itr->second;
			_items.erase(itr);
		}
		else
		{
			nRet = -2;
			//printf("no item is pop.\n");
		}

		return nRet;	
	}

	int pop(const std::string &key, T &item, const long long timeout)
	{	
		ThreadMutexGuard guard(_mutex);
		bool have_wait = false;
		int nRet = 0;
		
		while(true)
		{		
			typename std::map<std::string, T >::iterator itr = _items.find(key);
			if(itr != _items.end())
			{
				// 在超时时间内找到
				item = itr->second;
				_items.erase(itr);
				return 0;
			}

			if(have_wait == true)
			{
				// jump out of loop
				return nRet;
			}
			// 没找到			
			nRet = _cond.TimeWait(timeout);
			have_wait = true;
		}
		
		return nRet;

	}

	int pop_front(T &item)
	{		
		ThreadMutexGuard guard(_mutex);
		
		while(_items.empty())
		{
			int nRet = _cond.wait();
			if(nRet != 0)
			{
				CB_LOG_ERROR("cond wait failed.\n");
				return -1;
			}
		}
		
		item = _items.begin()->second;
		_items.erase(_items.begin());
		
		return 0;

	}

	int push(const std::string &key, T &item)
	{	
		ThreadMutexGuard guard(_mutex);
		
		_items.insert(std::make_pair(key, item));
		//唤醒所有等待的线程进行处理
		_cond.broadcast();
		
		return 0;
	}

private:
	std::map<std::string, T > _items;
	ThreadMutex _mutex;
	ThreadCond _cond;
};


}

#endif


