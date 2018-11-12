#ifndef __CPPBRICK_TSIGLETON_H__
#define __CPPBRICK_TSIGLETON_H__


#include <ThreadMutex.h>

namespace cppbrick {


template <typename T>
class TSingleton
{
public:
	static T* GetInstance()
	{
		if(m_pInstance == NULL)
		{
			m_singleton_lock.lock();
		
			if(m_pInstance == NULL)
			{
				m_pInstance = new T;
			}
			
			m_singleton_lock.unlock();
		}
		
		return m_pInstance;
	}

protected:
	TSingleton() 
	{
	}

public:
	static T *m_pInstance;
	static ThreadMutex m_singleton_lock;	
	
};
	

/*
在动态库中使用主程序单件对象
单件静态成员对象重新被分配和赋值为 NULL
*/
template <typename T>
T* TSingleton<T>::m_pInstance = NULL;

template <typename T>
ThreadMutex TSingleton<T>::m_singleton_lock;	

}
#endif 



