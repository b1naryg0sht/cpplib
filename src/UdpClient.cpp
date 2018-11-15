#include <sys/select.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include "UdpClient.h"
#include "SocketUtils.h"
//#include <Log.h>



namespace cppbrick {


UdpClient::UdpClient(): _fd(-1), _connect(false), _close(true), _ip(""), _port(0)
{

}


UdpClient::~UdpClient()
{
	close();
}


int UdpClient::connect(const std::string &ip, ushort_t port, bool doconnect)
{
	int nRet = 0;
	
	_fd = socket(AF_INET, SOCK_DGRAM, 0); 
	if(_fd < 0)
	{
		printf("socket(fd:%d) failed, errno:%d, errmsg:%s\n", _fd, errno, strerror(errno));
		return _fd;
	}


	SocketUtils::set_non_bolck(_fd);
	
	if(doconnect)
	{
		struct sockaddr_in addr;
		memset(&addr, 0x0, sizeof(sockaddr_in));
		addr.sin_family = AF_INET;
		inet_aton(ip.c_str(), &(addr.sin_addr));
		addr.sin_port = htons(port);
		nRet = ::connect(_fd, (struct sockaddr *)&addr, sizeof(addr));
		if(nRet != 0)
		{
			printf("connect(%s:%u) failed, errno:%d, errmsg:%s\n", 
					ip.c_str(), port, errno, strerror(errno));
			return nRet;
		}
		printf("connect(%s:%u) success.\n", ip.c_str(), port);
		_connect = true;

	}

	_ip = ip;
	_port = port;
	_close = false;

	return nRet;

}



int UdpClient::sendto(const char *buf, const uint_t len, int flags)
{
	int nRet = 0;

	if(_close)
	{
		printf("the udp client is closed.\n");
		return -1;
	}

	if(_connect)
	{
		nRet = ::sendto(_fd, buf, len, 0, NULL, 0);
		if(nRet < 0)
		{
			printf("sendto failed, fd:%d, ret:%d, errno:%d, errmsg:%s\n", _fd, nRet, errno, strerror(errno));
		}
	}
	else
	{
		struct sockaddr_in addr;
		memset(&addr, 0x0, sizeof(sockaddr_in));
		addr.sin_family = AF_INET;
		inet_aton(_ip.c_str(), &(addr.sin_addr));
		addr.sin_port = htons(_port);
		nRet = ::sendto(_fd, buf, len, 0, (struct sockaddr *)&addr, sizeof(addr));
		if(nRet < 0)
		{
			printf("sendto failed, fd:%d, ret:%d, errno:%d, errmsg:%s\n", _fd, nRet, errno, strerror(errno));
		}
	}

	return nRet;

}




int UdpClient::recvfrom(char *buf, const uint_t len)
{
	int nRet = 0;
	
	if(_close)
	{
		printf("the udp client is closed.\n");
		return -1;
	}

	int addrlen = 0;
	struct sockaddr_in addr;
	nRet = ::recvfrom(_fd, buf, len, 0, (struct sockaddr *)&addr, (socklen_t*)&addrlen);
	if(nRet < 0)
	{
		printf("recvfrom failed, fd:%d, ret:%d, errno:%d, errmsg:%s\n", _fd, nRet, errno, strerror(errno));
	}

	return nRet;
	
}



void UdpClient::close()
{
	if(!_close)
	{
		::close(_fd);
		_close = true;
	}
}


bool UdpClient::is_close()
{
	return _close;
}


int UdpClient::fd()
{
	return _fd;
}


}


