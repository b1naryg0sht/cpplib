#ifndef __CPPBRICK_EVENT_HANDLER_H__
#define __CPPBRICK_EVENT_HANDLER_H__

#include <memory>

namespace cppbrick {

class EventHandler;
//typedef std::shared_ptr<EventHandler>  EventHandlerPtr;
typedef EventHandler *  EventHandlerPtr;


class EventHandler
{	
public:
	EventHandler(){};

	virtual ~EventHandler(){};

	//handle_xxxx 成功返回0，失败返回非0
	
	//处理建立连接请求事件
	virtual int handle_accept(int fd){return 0;};

	//处理读事件
	virtual int handle_input(int fd){return 0;};

	//处理写事件
	virtual int handle_output(int fd){return 0;};

	//处理带外数据事件
	virtual int handle_exception(int fd){return 0;};

	//处理连接关闭事件
	virtual int handle_close(int fd){return 0;};

	//udp reactor 使用
	virtual int handle_input(const std::string &ip, unsigned short port, char *buf, unsigned int len){return 0;};

	//处理udp 关闭事件(模拟)
	virtual int handle_close(const std::string &ip, unsigned short port){return 0;};

	/*
	这个接口子类必须实现, 样例如下:
	virtual Event_Handler* renew(){return new Event_Handler;};
	*/
	virtual EventHandlerPtr renew() = 0;
	
};


}

#endif


