#ifndef __CPPBRICK_TCP_CLIENT_EPOLL_H__
#define __CPPBRICK_TCP_CLIENT_EPOLL_H__

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/epoll.h>

#include "Thread.h"
#include "EventHandler.h"
#include "ThreadMutex.h"


namespace cppbrick {

//一个TCP_Client 一个线程
class TCP_Client_Epoll : public Thread
{
public:
	/*
	handler是由外部应用new 产生，由TCP_Client 自动释放
	[注意]: 
	(1)如果使用长连接需要new TCP_Cient，外部应用不能
	delete它；
	(2)如果使用短链接需要外部应用自己保证释放它；
	
	*/
	TCP_Client_Epoll(EventHandlerPtr handler, bool asyn=false);

	virtual ~TCP_Client_Epoll();

	int open(const std::string &ip, unsigned short port, unsigned int timeout=1000000);

	int send_msg(const char *buf, unsigned int &len, int flags=0, unsigned int timeout=1000000);

	int rcv_msg(char *buf, unsigned int &len, unsigned int timeout=1000000);

	void close();

	bool is_close();

	int fd();

private:
	virtual int prepare();

	virtual int svc();

	virtual int do_init(void *args);
	
private:
	int epoller_create(int epoll_size);

	int epoller_ctl(int fd, int op, unsigned int events);

private:
	EventHandlerPtr _handler;
	std::string _ip;
	unsigned _port;
	int _fd;
	bool _asyn;
	bool _open;

	int _epfd;
	struct epoll_event *_ep_events;
	int _epoll_size;

	ThreadMutex _mutex;
};

}

#endif


