#include <iostream>
#include <unistd.h>
#include "SelectTimer.h"

using namespace cppbrick;

class MyTimerHandler : public SelectTimerHandler 
{
	virtual int handle_timeout(void *args = NULL)
	{
		printf("my timer timeout\n");
		return 0;
	}
};

int main()
{

	SelectTimer *timer = new SelectTimer;
	timer->register_timer_handler(new MyTimerHandler(), 3000000);
	timer->start();
	sleep(20);
	timer->stop();
	sleep(1);
	return 0;
}
