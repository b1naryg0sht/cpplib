#include <stdio.h>
#include <sys/select.h>
#include <errno.h>
#include <string.h>
#include "SelectTimer.h"
#include "Utils.h"

namespace cppbrick {



SelectTimer::SelectTimer(): _handler(NULL)
{


}



SelectTimer::~SelectTimer()
{
	DELETE_POINTER(_handler);
}



int SelectTimer::register_timer_handler(SelectTimerHandler *handler, unsigned long interval)
{
	int nRet = 0;

	if(handler == NULL)
	{
		printf("handler == NULL\n");
		return -1;
	}

	if(_handler != NULL)
	{
		printf("already register timer handler ago.\n");
		return -2;
	}

	if(interval == 0)
	{
		printf("interval == 0.\n");
		return -3;	
	}

	_sec = interval/1000000;
    _usec = interval%1000000;
	_handler = handler;
	
	return nRet;

}

int SelectTimer::set_interval(unsigned long interval)
{
	if(interval == 0)
	{
		printf("interval == 0.\n");
		return -3;	
	}

	_sec = interval/1000000;
	_usec = interval%1000000;

	return 0;
}


int SelectTimer::svc()
{
	int nRet = 0;

	timeval timeout;
	timeout.tv_sec = _sec;
	timeout.tv_usec = _usec;
	
	nRet = select(1, NULL, NULL, NULL, &timeout);
	if(nRet > 0)
	{		
	}
	else if(nRet == 0)
	{
		//printf("select timeout, errno:%d, errmsg:%s\n", errno, strerror(errno));
		_handler->handle_timeout();
	}
	else
	{
		if(errno == EINTR)
		{
			printf("select is interrupt, errno:%d, errmsg:%s\n", errno, strerror(errno));
			return 0;
		}
		else if(errno == EINVAL)
		{
			printf("nfds is negative or the value contained within timeout is invalid.\n");
			return -1;
		}
		else
		{
			printf("select failed, errno:%d, errmsg:%s\n", errno, strerror(errno));
			return -2;
		}

	}
			
	return 0;
	
}

int SelectTimer::start()
{
	int ret = 0;
	ret = init();
	if(ret != 0)
	{
		printf("Select_Timer init fail.\n");
		return ret;
	}
	ret = run();
	if(ret != 0)
	{
		printf("Select_Timer run fail.\n");
		return ret;
	}
	
	return 0;
}

}


