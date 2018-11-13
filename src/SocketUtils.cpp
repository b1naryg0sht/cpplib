#include <sys/ioctl.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
//#include "Log.h"
#include "SocketUtils.h"


#define CB_LOG_ERROR printf

namespace cppbrick {

int SocketUtils::set_non_bolck(int fd)
{
	int flags = 0;
	
	flags = fcntl(fd, F_GETFL, 0);
	if(flags < 0)
	{
		CB_LOG_ERROR("fcntl F_GETFL failed, fd:%d. errno:%d, errmsg:%s\n", fd, errno, strerror(errno));
		return -1;
	}

	flags |= O_NONBLOCK;
	if(fcntl(fd, F_SETFL, flags) < 0)
	{
		CB_LOG_ERROR("fcntl F_SETFL O_NONBLOCK failed, fd:%d. errno:%d, errmsg:%s\n", fd, errno, strerror(errno));
		return -2;
	}

	return 0;
	
}



int SocketUtils::set_bolck(int fd)
{
	int flags = 0;
	
	flags = fcntl(fd, F_GETFL, 0);
	if(flags < 0)
	{
		CB_LOG_ERROR("fcntl F_GETFL failed, fd:%d. errno:%d, errmsg:%s\n", fd, errno, ::strerror(errno));
		return -1;
	}

	flags &= ~O_NONBLOCK;
	if(fcntl(fd, F_SETFL, flags) < 0)
	{
		CB_LOG_ERROR("fcntl F_SETFL O_NONBLOCK failed, fd:%d. errno:%d, errmsg:%s\n", fd, errno, ::strerror(errno));
		return -2;
	}

	return 0;

}



int SocketUtils::set_nodelay(int fd)
{
	int nRet = 0;
	
	int opt = 1;
	nRet = setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &opt, sizeof(opt));
	if(nRet < 0)
	{
		CB_LOG_ERROR("setsockopt TCP_NODELAY failed, fd:%d. errno:%d, errmsg:%s\n", fd, errno, strerror(errno));
		return -1;
	}

	return nRet;

}

int SocketUtils::set_quickack(int fd)
{
	int nRet = 0;

	int opt = 1;
	nRet = setsockopt(fd, IPPROTO_TCP, TCP_QUICKACK, &opt, sizeof(opt));
	if(nRet < 0)
	{
		CB_LOG_ERROR("setsockopt TCP_QUICKACK failed, fd:%d. errno:%d, errmsg:%s\n", fd, errno, strerror(errno));
		return -1;
	}

	return nRet;

}



int SocketUtils::set_reuseaddr(int fd)
{
	int nRet = 0;
	
	int opt = 1;
	nRet = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	if(nRet < 0)
	{
		CB_LOG_ERROR("setsockopt SO_REUSEADDR failed, fd:%d. errno:%d, errmsg:%s\n", fd, errno, strerror(errno));
		return -1;
	}

	return nRet;
}



int SocketUtils::set_keepalive(int fd)
{
	int nRet = 0;
	
	int opt = 1;
	nRet = setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &opt, sizeof(opt));
	if(nRet < 0)
	{
		CB_LOG_ERROR("setsockopt SO_KEEPALIVE failed, fd:%d. errno:%d, errmsg:%s\n", fd, errno, strerror(errno));
		return -1;
	}

	return nRet;

}



int SocketUtils::set_closeonexec(int fd)
{
	int flags = 0;
	
	flags = fcntl(fd, F_GETFL, 0);
	if(flags < 0)
	{
		CB_LOG_ERROR("fcntl F_GETFL failed. errno:%d, errmsg:%s\n", errno, strerror(errno));
		return -1;
	}

	flags |= FD_CLOEXEC;
	if(fcntl(fd, F_SETFL, flags) < 0)
	{
		CB_LOG_ERROR("fcntl F_SETFL FD_CLOEXEC failed, fd:%d. errno:%d, errmsg:%s\n", fd, errno, strerror(errno));
		return -2;
	}

	return 0;


}



int SocketUtils::set_rcvbuf(int fd, int size)
{
	int nRet = 0;

	nRet = setsockopt(fd, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size));
	if(nRet < 0)
	{
		CB_LOG_ERROR("setsockopt SO_RCVBUF failed, fd:%d. errno:%d, errmsg:%s\n", fd, errno, strerror(errno));
		return -1;
	}

	return nRet;

}

int SocketUtils::set_sndbuf(int fd, int size)
{
	int nRet = 0;

	nRet = setsockopt(fd, SOL_SOCKET, SO_SNDBUF, &size, sizeof(size));
	if(nRet < 0)
	{
		CB_LOG_ERROR("setsockopt SO_SNDBUF failed, fd:%d. errno:%d, errmsg:%s\n", fd, errno, strerror(errno));
		return -1;
	}

	return nRet;
}




int SocketUtils::set_rcv_timeout(int fd, int timeout)
{
	int nRet = 0;

	timeval tv;
	tv.tv_sec = timeout;
	tv.tv_usec = 0;	
	nRet = setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
	if(nRet < 0)
	{
		CB_LOG_ERROR("setsockopt SO_RCVTIMEO failed, fd:%d. errno:%d, errmsg:%s\n", fd, errno, strerror(errno));
		return -1;
	}

	return nRet;

}




int SocketUtils::set_snd_timeout(int fd, int timeout)
{
	int nRet = 0;

	timeval tv;
	tv.tv_sec = timeout;
	tv.tv_usec = 0;
	nRet = setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
	if(nRet < 0)
	{
		CB_LOG_ERROR("setsockopt SO_SNDTIMEO failed, fd:%d. errno:%d, errmsg:%s\n", fd, errno, strerror(errno));
		return -1;
	}

	return nRet;

}




int SocketUtils::shutdown(int fd, int flag)
{
	int nRet = 0;

	nRet = shutdown(fd, flag);
	if(nRet < 0)
	{
		CB_LOG_ERROR("shutdown failed, fd:%d, flag:%d. errno:%d, errmsg:%s\n", fd, flag, errno, strerror(errno));
		return -1;
	}

	return nRet;
}



int SocketUtils::close_socket(int fd)
{
	return close(fd);
}


int SocketUtils::get_local_socket(int fd, std::string &ip, unsigned short &port)
{
	int nRet = 0;

	sockaddr addr;
	socklen_t len = (socklen_t)(sizeof(addr));
	memset(&addr, 0x0, sizeof(addr));
	nRet = getsockname(fd ,&addr, &len);
	if(nRet != 0)
	{
		CB_LOG_ERROR("getsockname failed, fd:%d, errno:%d, errmsg:%s\n", fd, errno, strerror(errno));
	}
	else
	{
		ip = inet_ntoa(((sockaddr_in*)&addr)->sin_addr);
		port = ntohs(((sockaddr_in*)&addr)->sin_port);
	}

	return nRet;
	
}


int SocketUtils::get_remote_socket(int fd, std::string &ip, unsigned short &port)
{
	int nRet = 0;

	sockaddr addr;
	socklen_t len = (socklen_t)(sizeof(addr));
	memset(&addr, 0x0, sizeof(addr));

	nRet = getpeername(fd ,&addr, &len);
	if(nRet != 0)
	{
		//printf("getpeername failed, fd:%d, errno:%d, errmsg:%s\n", fd, errno, strerror(errno));
	}
	else
	{
		ip = inet_ntoa(((sockaddr_in*)&addr)->sin_addr);
		port = ntohs(((sockaddr_in*)&addr)->sin_port);
	}

	return nRet;

}


}


