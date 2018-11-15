#include <iostream>
#include <unistd.h>
#include "Thread.h"
#include "TimeStamp.h"
#include "ThreadMeet.h"

using namespace cppbrick;

ThreadMeet thread_meet(3);


class ThreadLocal : public Thread {
public:
	virtual int svc()
	{
		TimeStamp ts1;
		printf("i am thread:%lx, I arrive at %ld:%ld\n", pthread_self(), ts1.getSeconds(), ts1.getUSeconds());
		thread_meet.Meet();
		TimeStamp ts2;
		printf("i am thread:%lx, I leave at %ld:%ld\n", pthread_self(), ts2.getSeconds(), ts2.getUSeconds());
		pthread_exit(NULL);
		return 0;
	}
};

int main()
{

	ThreadLocal a;
	ThreadLocal b;
	ThreadLocal c;
	a.init();
	a.run();
	sleep(1);
	
	b.init();
	b.run();
	#if 1
	sleep(1);
	c.init();
	c.run();

	sleep(1);
	#endif
	
	//a.stop();
	//b.stop();
	//c.stop();
	return 0;
}
