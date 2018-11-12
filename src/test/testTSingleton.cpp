#include <iostream>
#include <Thread.h>
#include <unistd.h>
#include <TSingleton.h>

using namespace cppbrick;

class Consumer : public Thread {
public:
	virtual int svc()
	{
		std::cout << "hello,world" << std::endl;
		sleep(1);
		return 0;
	}
};

#define PSGT_CON TSingleton<Consumer>::GetInstance()

using namespace cppbrick;

int main()
{

	PSGT_CON->init(NULL, 3);
	PSGT_CON->run();

	sleep(3);

	PSGT_CON->stop();
	// let the pro and con thread completely exit
	sleep(1);
	return 0;
}
