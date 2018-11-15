#ifndef __CPPBRICK_SelectTimer_H__
#define __CPPBRICK_SelectTimer_H__

#include "Thread.h"

namespace cppbrick {


//外部应用需要继承
class SelectTimerhandler
{
public:
	SelectTimerhandler() {}

	virtual ~SelectTimerhandler(){}

	virtual int handle_timeout(void *args = NULL) {return 0;}
};




//单线程定时器
class SelectTimer : public Thread
{
public:
	SelectTimer();

	~SelectTimer();

	//handler必须由外部应用通过new 创建， 不需要关心它的释放
	int register_timer_handler(SelectTimerhandler *handler, unsigned long interval=1000000);

	int set_interval(unsigned long interval);

	unsigned long get_interval()
	{
		return 1000000*_sec + _usec;
	}

	int start();

private:
	virtual int svc();
	
private:
	SelectTimerhandler  *_handler;
    long _sec;         //seconds
    long _usec;        //microseconds   微秒	

};

	
}

#endif


