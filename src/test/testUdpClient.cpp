#include <iostream>
#include <Thread.h>
#include <unistd.h>
#include <TVector.h>

using namespace cppbrick;

TVector<int> g_id_queue;

class Consumer : public Thread {
public:
	virtual int svc()
	{
		int e;
		g_id_queue.pop(e);
		CB_LOG_DEBUG("thread 0x%lx consume %d\n", thread_id(), e);
		return 0;
	}
};


class Producer : public Thread {
public:
	virtual int svc()
	{
		int i=0;
		while(1)
		{
			CB_LOG_DEBUG("thread 0x%lx produce %d\n", thread_id(), i);
			g_id_queue.push(i);
			sleep(1);
			i++;
		}
		return 0;
	}
};

using namespace cppbrick;

int main()
{

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
