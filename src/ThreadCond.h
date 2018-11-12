#ifndef __CPPBRICK_THREADCOND_H__
#define __CPPBRICK_THREADCOND_H__

#include <pthread.h>
#include "ThreadMutex.h"

namespace cppbrick {

class ThreadCond
{
public:

    ThreadCond(ThreadMutex &mutex);
	
    ~ThreadCond();

    int wait();

	//millisecond 是相对时间，内部会将其转化成绝对时间(单位: 毫秒)
    int TimeWait(long msec);

    int signal();

    int broadcast();

private:
    pthread_cond_t _cond;
	ThreadMutex &_mutex;
	
};

}
#endif

