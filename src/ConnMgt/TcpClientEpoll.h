#ifndef __CPPBRICK_TCP_CLIENT_EPOLL_H__
#define __CPPBRICK_TCP_CLIENT_EPOLL_H__

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/epoll.h>

#include "EventHandler.h"
#include "ThreadMutex.h"
#include "Conn.h"

namespace cppbrick {

class TcpClientEpoll : public Conn
{
public:
	//TcpClientEpoll(){}
	TcpClientEpoll(const StSvr &svr, bool asyn=false, EventHandlerPtr handler=NULL);

	virtual ~TcpClientEpoll();
	
	virtual int connect();

	virtual void close();

	virtual bool is_close();

	virtual int send(const char *buf, unsigned int &len, int flags=0, unsigned int timeout=100000);

	// sync rcv
	virtual int rcv(char *buf, unsigned int &len, unsigned int timeout=100000);

	bool do_stop_epoll();
	int tcp_epoll_cb();
private:

	int do_init();
	
	int epoller_create(int epoll_size);

	int epoller_ctl(int fd, int op, unsigned int events);

private:
	EventHandlerPtr _handler;
	int _fd;
	bool _asyn;
	bool _open;

	bool _stop_epoll;
	int _epfd;
	struct epoll_event *_ep_events;
	int _epoll_size;

	ThreadMutex _mutex;
};

}

#endif


