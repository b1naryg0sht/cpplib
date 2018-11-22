#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/resource.h>
#include "TcpClientEpoll.h"
#include "SocketUtils.h"
#include "OSUtils.h"
#include "Utils.h"
#include "Log.h"

namespace cppbrick {

static void *epoll_thread_cb(void *arg)
{
	TcpClientEpoll *client = (TcpClientEpoll *)arg;
	while(1)
	{	
		client->tcp_epoll_cb();
	}	
}

TcpClientEpoll::TcpClientEpoll(const StSvr &svr, bool asyn, EventHandlerPtr handler)
			: Conn(svr), _fd(-1), _open(false), 
			  _asyn(asyn), _handler(handler),  _stop_epoll(false), 
			  _epfd(-1), _ep_events(NULL), _epoll_size(0)
{

}


TcpClientEpoll::~TcpClientEpoll()
{
	close();

	if(_asyn)
	{
		DELETE_POINTER(_handler);
		DELETE_POINTER_ARR(_ep_events);	
	}
}



/*
同步TCP Client的一个问题是如果是对端关闭连接， 本端是不知道连接关闭的
除非本端发送消息才能检测到。
*/
int TcpClientEpoll::connect()
{
	int nRet = 0;
	
	if(_open)
	{
		CB_LOG_WARN("the tcp client is open before!\n");
		return -1;
	}
	
	_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(_fd < 0)
	{
		CB_LOG_ERROR("socket(fd:%d) failed, errno:%d, errmsg:%s\n", _fd, errno, strerror(errno));
		_fd = -1;
		return _fd;
	}


	nRet = SocketUtils::connect_s(_fd, _svr._ip, _svr._port, 1);
	if(nRet != 0)
	{
		//CB_LOG_ERROR("connect failed, errno:%d, errmsg:%s\n", errno, strerror(errno));
		::close(_fd);
		_fd = -1;
		return nRet;
	}


	if(_asyn)
	{
		nRet = do_init();
		if(nRet != 0)
		{
			CB_LOG_ERROR("init tcp client failed, ret:%d\n", nRet);
			::close(_fd);
			_fd = -1;
	
			return nRet;
		}

		pthread_t pid;
		nRet = pthread_create(&pid, NULL, epoll_thread_cb, this);
		if(nRet != 0)
		{
			CB_LOG_ERROR("run tcp client failed, ret:%d\n", nRet);

			TcpClientEpoll::epoller_ctl(_fd, EPOLL_CTL_DEL, 0);
			::close(_epfd);
			_epfd = -1;	
				
			::close(_fd);
			_fd = -1;
			
			return nRet;
		}

		pthread_detach(pid);
		
	}

	if(nRet == 0)
	{
		_open = true;
	}
	
	return nRet;
	
}



int TcpClientEpoll::send(const char *buf, unsigned int &len, int flags, unsigned int timeout)
{
	int nRet = 0;
	
	if(!_open)
	{
		CB_LOG_WARN("the tcp client is open ago!\n");
		return -1;
	}

	ThreadMutexGuard guard(_mutex);

	nRet = SocketUtils::send_n(_fd, buf, len, flags, timeout);
	
	return nRet;
}


int TcpClientEpoll::rcv(char *buf, unsigned int &len, unsigned int timeout)
{
	int nRet = 0;

	if(!_open)
	{
		CB_LOG_WARN("the tcp client is open ago!\n");
		return -1;
	}
		
	if(!_asyn)
	{
		ThreadMutexGuard guard(_mutex);
		nRet = SocketUtils::recv(_fd, buf, len, timeout);
	}
	else
	{
		CB_LOG_ERROR("the socket is asyn mode. it is useless.\n");
		nRet = -2;
	}

	return nRet;
	
}



void TcpClientEpoll::close()
{
	if(!_open)
	{
		return;
	}
	
	CB_LOG_DEBUG("close tcp client(%s:%u)!\n", _svr._ip.c_str(), _svr._port);

	if(_asyn)
	{
		_stop_epoll = true;
		
		TcpClientEpoll::epoller_ctl(_fd, EPOLL_CTL_DEL, 0);
		
		::close(_epfd);
		_epfd = -1;	
	}

	::close(_fd);
	_open = false;
	_fd = -1;
}


bool TcpClientEpoll::is_close()
{
	return !_open;
}

bool TcpClientEpoll::do_stop_epoll()
{
	return _stop_epoll;
}

int TcpClientEpoll::tcp_epoll_cb()
{
	int nRet = 0;
	
	int fd_cnt = epoll_wait(_epfd, _ep_events, _epoll_size, 1000);
	if(fd_cnt >0)
	{
		for(int i=0; i<fd_cnt; i++)
		{
			if(_ep_events[i].events & EPOLLIN )
			{
				//处理输入事件
				nRet = _handler->handle_input(_ep_events[i].data.fd);
				if(nRet != 0)
				{
					std::string local_ip = "";
					unsigned short local_port = 0;
					SocketUtils::get_local_socket(_fd, local_ip, local_port);
					
					std::string remote_ip = "";
					unsigned short remote_port = 0; 
					SocketUtils::get_remote_socket(_fd, remote_ip, remote_port);
					
					CB_LOG_DEBUG("handle_input failed! prepare to close(fd:%d), %s:%u --> %s:%u\n", 
						_fd, local_ip.c_str(), local_port, remote_ip.c_str(), remote_port);
					
					TcpClientEpoll::epoller_ctl(_fd, EPOLL_CTL_DEL, 0);
					_handler->handle_close(_fd);
					close();

				}
				
			}
			else
			{
				CB_LOG_ERROR("other events is arrived.\n");
			}
			
		}
		
	}
	else if(fd_cnt == 0)
	{
		//CB_LOG_ERROR("epoll_wait timeout, errno:%d, errmsg:%s\n", errno, strerror(errno));
	}
	else
	{
		if(errno == EINTR)
		{
			CB_LOG_ERROR("epoll_wait is interrupt, errno:%d, errmsg:%s\n", errno, strerror(errno));
		}
		else
		{
			CB_LOG_ERROR("epoll_wait failed, errno:%d, errmsg:%s\n", errno, strerror(errno));
			return -1;
		} 
	}

	return 0;


}
	


int TcpClientEpoll::do_init()
{
	int nRet = 0;

	if(_asyn)
	{
		//获取系统进程最大句柄数
		struct rlimit rlim;
		nRet = OSUtils::get_rlimit(RLIMIT_NOFILE, &rlim);
		if(nRet != 0)
		{
			CB_LOG_ERROR("get_rlimit failed, ret:%d\n", nRet);
			return nRet;
		}
		//CB_LOG_ERROR("get_rlimit RLIMIT_NOFILE, rlim_cur:%d\n", (int)rlim.rlim_cur);

		//计算出最大的nfds
		int epoll_size = rlim.rlim_cur;
		nRet = TcpClientEpoll::epoller_create(epoll_size);
		if(nRet != 0)
		{
			CB_LOG_ERROR("epoller_create failed, ret:%d\n", nRet);
			return nRet;
		}

		nRet = TcpClientEpoll::epoller_ctl(_fd, EPOLL_CTL_ADD, EPOLLIN);
		if(nRet != 0)
		{
			CB_LOG_ERROR("epoller_create failed, ret:%d\n", nRet);
			return nRet;
		}		
		
	}
	else
	{
		CB_LOG_ERROR("it isn't asyn tcp client.\n");
		return -1;
	}

	//阻塞进程信号SIGINT
	nRet = OSUtils::signal_mask(SIG_BLOCK, 1, SIGINT);
	if (nRet != 0)
	{
		CB_LOG_ERROR("signal_mask failed. ret:%d, errno:%d, errmsg:%s\n", nRet, errno, strerror(errno));
	}	
	
	return nRet;
	
}
	


int TcpClientEpoll::epoller_create(int epoll_size)
{
	int nRet = 0;
	
	_epfd = epoll_create(epoll_size);
	if(_epfd == -1)
	{
		CB_LOG_ERROR("epoll_create failed, errno:%d, errmsg:%s\n", errno, strerror(errno));
		return -1;
	}
	
	_epoll_size = epoll_size;
	_ep_events = new struct epoll_event[_epoll_size];
	
	return nRet;
}




int TcpClientEpoll::epoller_ctl(int fd, int op, unsigned int events)
{
	int nRet = 0;

	struct epoll_event ep_ev;
	ep_ev.data.fd = fd;
	ep_ev.events = events;
	nRet = epoll_ctl(_epfd, op, fd, &ep_ev); 
	if(nRet != 0)
	{
		CB_LOG_ERROR("epoll_ctl failed, fd:%d, errno:%d, errmsg:%s\n", fd, errno, strerror(errno));
	}

	return nRet;
	
}



}


