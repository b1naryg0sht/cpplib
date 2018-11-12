#ifndef __CPPBRICK_TMAPINT_H__
#define __CPPBRICK_TMAPINT_H__

#include <map>
#include <ThreadMutex.h>
#include <ThreadCond.h>

namespace cppbrick {

template <typename T>
class TMapInt
{
public:
	TMapInt() 
	{
	}

	~TMapInt()
	{
	}

	unsigned int size()
	{
		ThreadMutexGuard guard(_mutex);
		return _items.size();
	}

	bool empty()
	{
		ThreadMutexGuard guard(_mutex);
		return _items.empty();
	}

	bool get(const int key, T &value)
	{
		ThreadMutexGuard guard(_mutex);
		
		typename std::map<int, T >::iterator itr = _items.find(key);
		if(itr != _items.end())
		{
			//在超时时间内找到
			value = itr->second;
			return true;
		}
		
		return false;
	}

	void insert(const int key, T &value)
	{
		ThreadMutexGuard guard(_mutex);
		_items.insert(std::make_pair(key, value));
	}

	void del(const int key)
	{
		ThreadMutexGuard guard(_mutex);
		
		typename std::map<int, T >::iterator itr = _items.find(key);
		if(itr != _items.end())
		{
			_items.erase(itr);
		}

	}

	void clear()
	{
		ThreadMutexGuard guard(_mutex);
		_items.clear();
	}

private:
	std::map<int, T > _items;
	ThreadMutex _mutex;
	//ThreadCond _cond;
};

}


#endif


