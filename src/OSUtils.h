#ifndef __YOGURT__THREAD_UTILS_H__
#define __YOGURT__THREAD_UTILS_H__

#include <pthread.h>

namespace Yogurt {

	/** 
	 * returns the thread ID
	 **/
	std::string GetThreadId();

}

#endif //__YOGURT__THREAD_UTILS_H__
