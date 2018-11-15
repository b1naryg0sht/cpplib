#include <iostream>
#include <unistd.h>
#include "Thread.h"
#include "TVector.h"

/*
* test TVector rate limit feature
*/

using namespace cppbrick;

#define THREAD_NUM 2

TVector<int> g_id_queue(1000000, THREAD_NUM*4);

//consume 5000 packages per second
class Consumer : public Thread {
public:
	virtual int svc()
	{
		int e;
		TimeStamp ts;
		g_id_queue.pop(e);
		usleep(2000);
		//CB_LOG_DEBUG("thread 0x%lx consume %d at %ld:%ld\n", thread_id(), e, ts.getSeconds(), ts.getUSeconds());
		return 0;
	}
};


//procude 10000 packages per second
class Producer : public Thread {
public:
	virtual int svc()
	{
		int i=0;
		while(1)
		{
			int ret = g_id_queue.push(i);
			if(ret != 0)
			{
				CB_LOG_WARN("queue full. sleep 1 second.\n");
				sleep(1);
			}
			//CB_LOG_DEBUG("thread 0x%lx produce %d\n", thread_id(), i);
			usleep(100);
			i++;
		}
		return 0;
	}
};


using namespace cppbrick;

int main()
{
	Consumer con;
	con.init(NULL, THREAD_NUM);
	con.run();

	Producer pro;
	pro.init();
	pro.run();


	sleep(20);
	pro.stop();
	con.stop();
	// let the pro and con thread completely exit
	sleep(3);
	return 0;
}
