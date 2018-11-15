#include <iostream>
#include <unistd.h>
#include "Thread.h"
#include "UdpReactor.h"
#include "EventHandler.h"
#include "SocketUtils.h"

using namespace cppbrick;

class Req_UDP_Event_Handler : public EventHandler
{	
public:
	Req_UDP_Event_Handler() {}

	virtual ~Req_UDP_Event_Handler(){}

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
		
		return nRet;

	}

	virtual int handle_input(const std::string &ip, unsigned short port, char *buf, unsigned int len)
	{
		int nRet = 0;
		
		printf("rev %s from %s:%d\n", buf, ip.c_str(), port);

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
			
		return nRet;

	}

	virtual EventHandlerPtr renew()
	{
		return new Req_UDP_Event_Handler;
	}
	
private:
	std::string _buf;
	
};



int main()
{

	StUdpReactorAgrs args;
	args.port = 17958;

	UdpReactor udpReactor(new Req_UDP_Event_Handler);
	udpReactor.init(&args);
	udpReactor.run();

	while(1) 
	{
		sleep(100);
	}
	return 0;
}
