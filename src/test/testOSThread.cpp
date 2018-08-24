#include <iostream>
#include <os_thread.h>
#include <unistd.h>

using namespace Yogurt;

int main()
{
	class TestThread : public OSThread {
	public:
		virtual int svc()
		{
			std::cout << "test OSThread" << std::endl;
			return 0;
		}
	};

	TestThread t;
	t.init();
	t.run();
	//usleep(10);
	sched_yield();
	t.stop();
}
