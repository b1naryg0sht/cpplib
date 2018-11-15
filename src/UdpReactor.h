#ifndef __CPPBRICK_UDP_REACTOR_H__
#define __CPPBRICK_UDP_REACTOR_H__

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <map>
#include "Thread.h"
#include "EventHandler.h"
#include "ThreadMutex.h"


namespace cppbrick {


struct StUdpReactorAgrs
{
	std::string ip;
	unsigned short port;
	unsigned int epoll_size;

	StUdpReactorAgrs()
	{
		ip = "";
		port = 3300;
		epoll_size = 10;
	}
};


/*
UDP Reactor 重要特性:
最大接收buf 尺寸 1024 字节
可以将同一个客户端发送的多个小数据段都传递给同一个event handler 处理。
应用需要自己处理udp 报文的集中和发送顺序问题。
由于UDP 无连接所以需要UDP 客户端发送尺寸为0的报文表示停止发送。
*/
class UdpReactor : public Thread
{
public:	
	//handler必须由外部应用通过new 创建， 不需要关心它的释放
	UdpReactor(EventHandlerPtr handler, unsigned int buf_size = 8096);

	virtual ~UdpReactor();
	
private:
	virtual int prepare();
	
	virtual int svc();

	virtual int do_init(void *args);
	
private:
	int epoller_create(int epoll_size);

	int epoller_ctl(int fd, int op, unsigned int events);

	int do_bind(const std::string &ip, unsigned short port);
		
	void release();
	
private:
	EventHandlerPtr _handler;
	std::map<std::string, EventHandler*> _handlers;

	int _fd;
	int _epfd;
	struct epoll_event *_ep_events;
	int _epoll_size;
	unsigned int _buf_size;
	char *_buf;
	
};

}

#endif


