#include <iostream>
#include <Thread.h>
#include <unistd.h>

using namespace cppbrick;

int main()
{
	class TestThread : public Thread {
	public:
		virtual int svc()
		{
			std::cout << "test Thread" << std::endl;
			return 0;
		}
	};

	TestThread t;
	t.init();
	t.run();
	t.stop();
	sleep(1);
}
