#ifndef _CONN_H
#define _CONN_H

#include <memory>

namespace cppbrick {

struct StSvr
{
   	std::string _ip;                   //服务IP
   	std::string _ip_out;               //公网IP地址
	unsigned short _port;          	   //服务端口
	std::string _domain;              //域名，针对http接口

	StSvr(): _ip(""), _ip_out(""), _port(0),_domain("")
	{
	}

};


/*
Conn 既可以是同步连接也可以是异步连接。
异步连接的好处就是对端关闭，该Conn 会立即知道。
同步连接在对端关闭的情况下，不会立即知道，在使用该Conn发送后回收到对端的RST 报文，
再使用该Conn接收会返回0(表明该Conn 已经关闭)，会从Conn Mgt中标识不可用。后面不会从Conn Mgt中获取到。
接口抽象有如下几个：
1.open
2.close
3.send
4.recv
5.get_conn_state
*/
class Conn
{
public:
	Conn(){}
	Conn(const StSvr &svr):_svr(svr){}
	
	virtual ~Conn() {}

	virtual int connect() = 0;

	virtual void close() = 0;

	virtual bool is_close() = 0;

	virtual int send(const char *buf, unsigned int &len, int flags=0, unsigned int timeout=300000) = 0;

	virtual int rcv(char *buf, unsigned int &len, unsigned int timeout=300000) = 0;
public:
	StSvr _svr;
};

typedef std::shared_ptr<Conn> ConnPtr;

}
#endif


