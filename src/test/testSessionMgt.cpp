#include <iostream>
#include <unistd.h>
#include "SessionMgt.h"
#include "TcpReactor.h"
#include "EventHandler.h"
#include "SocketUtils.h"

using namespace cppbrick;

class Req_TCP_Event_Handler : public EventHandler
{	
public:
	Req_TCP_Event_Handler() {}

	virtual ~Req_TCP_Event_Handler(){}

	virtual int handle_accept(int fd)
	{
		int nRet = 0;
		
		std::string local_ip;
		unsigned short local_port;
		SocketUtils::get_local_socket(fd, local_ip, local_port);
		
		std::string remote_ip;
		unsigned short remote_port; 
		SocketUtils::get_remote_socket(fd, remote_ip, remote_port);
		
		printf( "accept from access svr(fd:%d), %s:%u --> %s:%u\n", 
			fd, remote_ip.c_str(), remote_port, local_ip.c_str(), local_port);

		std::string sid;
		PSGT_SessionMgt->insert_session(fd, sid);		
		return nRet;

	}

	virtual int handle_input(int fd)
	{
		int nRet = 0;
		
		//获取前端ip 和port
		std::string ip = "";
		unsigned short port = 0;
		SocketUtils::get_remote_socket(fd, ip, port);
		
		//读取fd
		char buf[4096] = {0};
		unsigned int buf_len = 4095;
		nRet = SocketUtils::recv(fd, buf, buf_len, 300000);
		if(nRet == 0)
		{
			printf( "rcv close form access svr(fd:%d, peer:%s:%u), ret:%d\n", fd, ip.c_str(), port, nRet);
			return -1;
		}
		else if(nRet == 1)
		{
			//XCP_LOGGER_INFO(&g_logger, "rcv success from access svr(fd:%d), req(%u):%s\n", fd, buf_len, buf); 
		}
		else
		{
			printf("rcv failed from access svr(fd:%d, peer:%s:%u), ret:%d\n", fd, ip.c_str(), port, nRet);
			return 0;
		}
		buf[buf_len] = '\0';

		// parse your buf here
		return 0;
	}

	virtual int handle_close(int fd)
	{
		int nRet = 0;
		
		std::string local_ip = "";
		unsigned short local_port = 0;
		SocketUtils::get_local_socket(fd, local_ip, local_port);
		
		std::string remote_ip = "";
		unsigned short remote_port = 0; 
		SocketUtils::get_remote_socket(fd, remote_ip, remote_port);
		
		printf("close from access svr(fd:%d), %s:%u --> %s:%u\n", 
			fd, remote_ip.c_str(), remote_port, local_ip.c_str(), local_port);

		std::string sid;
		PSGT_SessionMgt->remove_session(fd, sid);
		return nRet;

	}

	virtual EventHandlerPtr renew()
	{
		return new Req_TCP_Event_Handler;
	}
	
private:
	std::string _buf;
	
};



int main()
{
	StTcpReactorAgrs args;
	args.port = 17958;

	TcpReactor tcpReactor(new Req_TCP_Event_Handler);
	tcpReactor.init(&args);
	tcpReactor.run();

	while(1) 
	{
		sleep(100);
	}

	return 0;
}
