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
#include "TimeStamp.h"


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

//在超时时间中循环接收指定长度的数据
int SocketUtils::recv_n(int fd, char *buf, unsigned int &len, const unsigned int timeout)
{
	int nRet = 1;

	unsigned int buf_size = len;
	len = 0;

	//设置为非阻塞
	set_non_bolck(fd);

	TimeStamp begints;
	unsigned long long begin_usec = begints.ToUSeconds(); //轮循开始时间
	unsigned long long end_usec = 0; //轮循结束时间
	
	while(true)
	{
		
		//计算循环花费时间
		if(end_usec != 0)
		{
			unsigned long long span_usec = end_usec - begin_usec;
			if(span_usec >= timeout)
			{
				printf("recv_n timeout.\n");
				return 2;
			}
		}
			
		int ret = ::recv(fd, buf+len, buf_size, 0);
		if(ret > 0)
		{
			len += ret;
			buf_size -= ret;
			if(buf_size == 0)
			{
				//收到指定长度的数据
				return 1;
			}
		}
		else if(ret == 0)
		{
			//recv返回0有两种可能: (1)对端fd关闭(2)传给recv 的size=0
			//printf("socket(%d) is close\n", fd);
			return 0;
		}
		else
		{
			//没收到数据或者被中断就进行下轮接收操作
			if((errno == EAGAIN)||(errno == EINTR)) 	
			{	
				//no oper
			}

			else
			{
				printf("recv_n(%d) failed, errno:%d, errmsg:%s\n", 
						fd, errno, strerror(errno));
				return ret;
			}
		}

		TimeStamp endts;
		end_usec = endts.ToUSeconds();

	}
	
	
	return nRet;

}




//在超时时间中仅接收一次指定长度的数据
int SocketUtils::recv(int fd, char *buf, unsigned int &len, const unsigned int timeout)
{
	int nRet = 1;

	unsigned int buf_size = len;
	len = 0;

	//设置为非阻塞
	set_non_bolck(fd);

	TimeStamp begints;
	unsigned long long begin_usec = begints.ToUSeconds(); //轮循开始时间
	unsigned long long end_usec = 0; //轮循结束时间
	
	while(true)
	{
		
		//计算循环花费时间
		if(end_usec != 0)
		{
			unsigned long long span_usec = end_usec - begin_usec;
			if(span_usec >= timeout)
			{
				printf("recv timeout.\n");
				return 2;
			}
		}
			
		int ret = ::recv(fd, buf+len, buf_size, 0);
		if(ret > 0)
		{
			len += ret;
			buf_size -= ret;

			return 1;

		}
		else if(ret == 0)
		{
			//recv返回0有两种可能: (1)对端fd关闭(2)传给recv 的size=0
			//printf("socket(%d) is close\n", fd);
			return 0;
		}
		else
		{
			//没收到数据或者被中断就进行下轮接收操作
			if((errno == EAGAIN)||(errno == EINTR)) 	
			{	
				//no oper
			}

			else
			{
				printf("recv(%d) failed, errno:%d, errmsg:%s\n", 
						fd, errno, strerror(errno));
				return ret;
			}
		}

		TimeStamp endts;
		end_usec = endts.ToUSeconds();

	}

	return nRet;
	

}





int SocketUtils::send_n(int fd, const char *buf, unsigned int &len, int flags, const unsigned int timeout)
{
	int nRet = 0;

	unsigned int buf_size = len;
	len = 0;

	//设置为非阻塞
	set_non_bolck(fd);

	TimeStamp begints;
	unsigned long long begin_usec = begints.ToUSeconds(); //轮循开始时间
	unsigned long long end_usec = 0; //轮循结束时间
	
	while(true)
	{
		//计算循环花费时间
		if(end_usec != 0)
		{
			unsigned long long span_usec = end_usec - begin_usec;
			if(span_usec >= timeout)
			{
				printf("send_n timeout.\n");
				return 2;
			}
		}
			
		int ret = ::send(fd, buf+len, buf_size, flags);
		if(ret >= 0)
		{
			len += ret;
			buf_size -= ret;
			if(buf_size == 0)
			{
				//发送指定长度的数据
				return 0;
			}
		}
		else
		{
			//没收到数据或者被中断就进行下轮接收操作
			if((errno == EAGAIN)||(errno == EINTR)) 	
			{	
				//no oper
			}
			else
			{
				printf("send_n(%d) failed, errno:%d, errmsg:%s\n", 
						fd, errno, strerror(errno));
				return ret;
			}
		}

		TimeStamp endts;
		end_usec = endts.ToUSeconds();

	}
	
	return nRet;

}




int SocketUtils::send(int fd, const char *buf, unsigned int &len, int flags, const unsigned int timeout)
{
	int nRet = 0;

	unsigned int buf_size = len;
	len = 0;

	//设置为非阻塞
	set_non_bolck(fd);

	TimeStamp begints;
	unsigned long long begin_usec = begints.ToUSeconds(); //轮循开始时间
	unsigned long long end_usec = 0; //轮循结束时间
	
	while(true)
	{
		//计算循环花费时间
		if(end_usec != 0)
		{
			unsigned long long span_usec = end_usec - begin_usec;
			if(span_usec >= timeout)
			{
				printf("send timeout.\n");
				return 2;
			}
		}
			
		int ret = ::send(fd, buf+len, buf_size, flags);
		if(ret >= 0)
		{
			len += ret;
			buf_size -= ret;

			return 0;
		}
		else
		{
			//没收到数据或者被中断就进行下轮接收操作
			if((errno == EAGAIN)||(errno == EINTR)) 	
			{	
				//no oper
			}
			else
			{
				printf("send(%d) failed, errno:%d, errmsg:%s\n", 
						fd, errno, strerror(errno));
				return ret;
			}
		}

		TimeStamp endts;
		end_usec = endts.ToUSeconds();

	}
	
	return nRet;

}





int SocketUtils::send_s(int fd, const char *buf, unsigned int &len, int flags)
{
	int total = (int)len;
	int ret = ::send(fd, buf, total, flags);
	if(ret >= total)
	{
		return 0;
	}
	else if((ret < total) && (ret >= 0))
	{
		len = (unsigned int)ret;
		printf("send(%d) timeout, total:%u, len:%u\n", fd, total, len);
		return 2;
	}
	else if(ret < 0)
	{
		printf("send(%d) failed, errno:%d, errmsg:%s\n", fd, errno, strerror(errno));
	}
	
	return -1;

}




int SocketUtils::connect(int fd, std::string ip, unsigned short port, const unsigned int timeout)
{
	int nRet = 0;
	
	long long sec = 0;
	long long usec = 0;
	if(timeout > 0)
	{
		sec = timeout/1000000;
		usec = timeout%1000000;

		set_non_bolck(fd);
	} 

	struct sockaddr_in addr;
	memset(&addr, 0x0, sizeof(sockaddr_in));
	addr.sin_family = AF_INET;
	inet_aton(ip.c_str(), &(addr.sin_addr));
	addr.sin_port = htons(port);
	socklen_t len = sizeof(addr);
	
	/*
	#include <sys/socket.h>
	int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen); 
	如果成功返回0；否则返回-1
	*/
	nRet = ::connect(fd, (sockaddr*)&addr, len);
	if(nRet < 0)
	{
		if (errno == EINPROGRESS)
		{
			fd_set write_set;
			FD_ZERO(&write_set);
			FD_SET(fd, &write_set);
			
			while(true)
			{
				struct timeval _timeout;	
				_timeout.tv_sec = sec;	
				_timeout.tv_usec = usec;
				
				nRet = select(fd+1, NULL, &write_set, NULL, &_timeout);
				if(nRet > 0)
				{
					int retval = 0;
					socklen_t len = sizeof(retval);
					nRet = getsockopt(fd, SOL_SOCKET, SO_ERROR, &retval, &len);
					if (nRet != 0)
					{
						printf("getsockopt failed, errno:%d, errmsg:%s\n", errno, strerror(errno));
						return nRet;
					}
					
					if (retval != 0)
					{
						printf("getsockopt SOL_SOCKET SO_ERROR, retval:%d\n", retval);
						return -1;
					}
					else
					{
						printf("connect(%s:%u) success during timeout.\n", ip.c_str(), port); 
						return 0;
					}
					
				}
				else if(nRet == 0)
				{
					printf("connect timeout, errno:%d, errmsg:%s\n", errno, strerror(errno));
					return 2;
				}
				else
				{
					if(errno == EINTR)
					{
						printf("connect is interrupt, errno:%d, errmsg:%s\n", errno, strerror(errno));
						continue;
					}
					else
					{
						printf("connect failed, errno:%d, errmsg:%s\n", errno, strerror(errno));
						return nRet;
					}
				}

			}		
			
		}
		else
		{
			printf("connect(%s:%u) failed, errno:%d, errmsg:%s\n", 
				ip.c_str(), port, errno, strerror(errno));
		}

	}
	else
	{
		printf("connect(%s:%u) success.\n", ip.c_str(), port);
	}

	return nRet;

}




int SocketUtils::connect_s(int fd, std::string ip, unsigned short port, const unsigned int timeout)
{
	int nRet = 0;
	
	struct sockaddr_in addr;
	memset(&addr, 0x0, sizeof(sockaddr_in));
	addr.sin_family = AF_INET;
	inet_aton(ip.c_str(), &(addr.sin_addr));
	addr.sin_port = htons(port);
	socklen_t len = sizeof(addr);
	
	/*
	#include <sys/socket.h>
	int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen); 
	如果成功返回0；否则返回-1
	*/
	nRet = ::connect(fd, (sockaddr*)&addr, len);
	if(nRet < 0)
	{
		//printf("connect(%s:%u) failed, errno:%d, errmsg:%s\n", ip.c_str(), port, errno, strerror(errno));
	}
	else
	{
		printf("connect(%s:%u) success.\n", ip.c_str(), port);
	}

	return nRet;

}

}


