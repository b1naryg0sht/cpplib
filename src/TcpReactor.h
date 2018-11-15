#ifndef __CPPBRICK_REACTOR_H__
#define __CPPBRICK_REACTOR_H__

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include "EventHandler.h"
#include "TMapInt.h"
#include "Thread.h"


namespace cppbrick {

struct StTcpReactorAgrs
{
	std::string ip;
	unsigned short port;
	unsigned int backlog;
	unsigned int epoll_size;

	StTcpReactorAgrs()
	{
		ip = "";
		port = 3300;
		backlog = 20480;
		epoll_size = 40960;
	}
	
};



class TcpReactor : public Thread
{
public:	
	//handler必须由外部应用通过new 创建， 不需要关心它的释放
	TcpReactor(EventHandlerPtr handler);

	virtual ~TcpReactor();

	int listen_fd();

	//关闭监听，停止网络io 服务
	void stop_listen();
	
private:
	virtual int prepare();
	
	virtual int svc();

	virtual int do_init(void *args);
	
private:
	int epoller_create(int epoll_size);

	int epoller_ctl(int fd, int op, unsigned int events);

	int do_listen(const std::string &ip, unsigned short port, int backlog);

	void release();

public:
	//该接口不允许在event_handler的回调函数中被调用，只能在其他的线程中被调用
	void del_fd(int fd);
	
private:
	EventHandlerPtr _handler;
	TMapInt<EventHandlerPtr> _handlers;

	int _listen_fd;
	int _epfd;
	struct epoll_event *_ep_events;
	int _epoll_size;
	
};


}

#endif


