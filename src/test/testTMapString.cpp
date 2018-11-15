#include <iostream>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include "Log.h"
#include "Thread.h"
#include "TMapString.h"

using namespace cppbrick;

TMapString<int> g_id_queue;
#define RANGE	100

class Consumer : public Thread {
public:
	virtual int svc()
	{
		int e = rand()%RANGE;
		int ret = g_id_queue.pop(std::to_string(e), e);
		//printf("ret = %d\t", ret);
		if(ret == 0)
		{
			CB_LOG_DEBUG("thread 0x%lx consume %d ok\n", thread_id(), e);
		}
		usleep(200000);
		return 0;
	}
};


class Producer : public Thread {
public:
	virtual int svc()
	{
		while(1)
		{
			int n = rand()%RANGE;
			
			int ret = g_id_queue.push(std::to_string(n), n);
			CB_LOG_DEBUG("thread 0x%lx produce %d ok\n", thread_id(), n);
			usleep(200000);
		}
		return 0;
	}
};


int main()
{
	srand(time(NULL));
	Consumer con;
	con.init(NULL, 3);
	con.run();

	Producer pro;
	pro.init();
	pro.run();
	

	sleep(10);
	pro.stop();
	con.stop();
	// let the pro and con thread completely exit
	sleep(1);
	return 0;
}
