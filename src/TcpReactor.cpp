#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/resource.h>
#include "TcpReactor.h"
#include "SocketUtils.h"
#include "Utils.h"
#include "OSUtils.h"

namespace cppbrick {


TcpReactor::TcpReactor(EventHandler *handler): _handler(handler), _listen_fd(-1),
	_epfd(-1), _ep_events(NULL), _epoll_size(0)
{

}


TcpReactor::~TcpReactor()
{
	release();
}



int TcpReactor::prepare()
{
	int nRet = 0;

	//阻塞进程信号SIGINT
	nRet = OSUtils::signal_mask(SIG_BLOCK, 1, SIGINT);
	if (nRet != 0)
	{
		printf("signal_mask failed. ret:%d, errno:%d, errmsg:%s\n", 
				nRet, errno, strerror(errno));
	}
	
	return nRet;
}


int TcpReactor::do_init(void *args)
{
	int nRet = 0;
	
	if(!args)
	{
		printf("agrs is NULL\n");
		return -1;
	}
	StTcpReactorAgrs *_args = (StTcpReactorAgrs *)args;

	//获取系统进程最大句柄数
	struct rlimit rlim;
	nRet = OSUtils::get_rlimit(RLIMIT_NOFILE, &rlim);
	if(nRet != 0)
	{
		printf("get_rlimit failed, ret:%d\n", nRet);
		return nRet;
	}
	printf("get_rlimit RLIMIT_NOFILE, rlim_cur:%d\n", (int)rlim.rlim_cur);

	//计算出最大的nfds
	int epoll_size = rlim.rlim_cur > (_args->epoll_size) ? (_args->epoll_size) : rlim.rlim_cur;
	
	nRet = TcpReactor::epoller_create(epoll_size);
	if(nRet != 0)
	{
		printf("epoller_create failed, ret:%d\n", nRet);
		return nRet;
	}

	nRet = TcpReactor::do_listen(_args->ip, _args->port, _args->backlog);
	if(nRet != 0)
	{
		printf("do_listen failed, ret:%d\n", nRet);
		return nRet;
	}
	
	return nRet;
	
}





void TcpReactor::stop_listen()
{
	::close(_listen_fd);
}



int TcpReactor::do_listen(const std::string &ip, unsigned short port, int backlog)
{
	int nRet = 0;
	
	struct sockaddr_in addr;
	bzero(&addr, sizeof(sockaddr_in));
	addr.sin_family = AF_INET;
	if(ip != "")
	{
		inet_aton(ip.c_str(), &(addr.sin_addr));
	}
	else
	{
		addr.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	addr.sin_port = htons(port);

	_listen_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(_listen_fd < 0)
	{
		printf("socket failed, errno:%d, errmsg:%s\n", errno, strerror(errno));
		return _listen_fd;
	}

	SocketUtils::set_reuseaddr(_listen_fd);

	//_listen_fd 设置为非阻塞后accept(_listen_fd)，如果此时没有连接就会报错，errno==EAGAIN
	SocketUtils::set_non_bolck(_listen_fd);
	
	//设置nodely
	SocketUtils::set_nodelay(_listen_fd);

	nRet = bind(_listen_fd, (sockaddr*)&addr, sizeof(addr));
	if(nRet != 0)
	{
		printf("bind(%s:%u) failed, errno:%d, errmsg:%s\n", 
			ip.c_str(), port, errno, strerror(errno));
		close(_listen_fd);
		return nRet;
	}

	nRet = listen(_listen_fd, backlog);
	if(nRet != 0)
	{
		printf("listen(%s:%u) failed, errno:%d, errmsg:%s\n", 
			ip.c_str(), port, errno, strerror(errno));
		close(_listen_fd);
		return nRet;
	}
	
	nRet = TcpReactor::epoller_ctl(_listen_fd, EPOLL_CTL_ADD, EPOLLIN);
	if(nRet != 0)
	{
		close(_listen_fd);
		return nRet;
	}
	
	printf("---- tcp listen (%s:%u) ----\n", ip.c_str(), port);
	
	return nRet;
	
}



void TcpReactor::release()
{
	printf("release reactor!\n");
	
	close(_epfd);
	_epfd = -1;
	DELETE_POINTER(_handler);
	DELETE_POINTER_ARR(_ep_events);
	_handlers.clear();
}




void TcpReactor::del_fd(int fd)
{
	printf("=== close success, fd:%d\n", fd);
	
	TcpReactor::epoller_ctl(fd, EPOLL_CTL_DEL, 0);
	::close(fd);
	_handlers.del(fd);

	printf("del fd from reactor, fd:%d\n", fd);
}




int TcpReactor::svc()
{
	int nRet = 0;

	int fd_cnt = epoll_wait(_epfd, _ep_events, _epoll_size, -1);
	if(fd_cnt >0)
	{
		for(int i=0; i<fd_cnt; i++)
		{
			if(_ep_events[i].events & EPOLLOUT )
			{
				//处理输出事件
				EventHandlerPtr handler;
				if(_handlers.get((int)_ep_events[i].data.fd, handler))
				{
					handler->handle_output(_ep_events[i].data.fd);	
				}
			}
			else if(_ep_events[i].events & EPOLLIN )
			{
				//处理输入事件
				if(_listen_fd == _ep_events[i].data.fd)
				{
					//处理新连接事件
					struct sockaddr_in client_addr;
					socklen_t len = sizeof(client_addr);
					int conn_fd = accept(_listen_fd, (sockaddr*)&client_addr, &len);
					if(conn_fd < 0)
					{
						printf("accept failed, conn_fd:%d, errno:%d, errmsg:%s\n", conn_fd, errno, strerror(errno));
					}
					else
					{	
						printf("accept success, conn_fd:%d\n", conn_fd);

						//设置发送超时时间
						SocketUtils::set_snd_timeout(conn_fd, 3);
						
						EventHandlerPtr handler = _handler->renew();
						nRet = handler->handle_accept(conn_fd);
						if(nRet != 0)
						{
							printf("--- handle_accept failed, ret:%d\n", nRet);
							close(conn_fd);
						}
						else
						{
							nRet = TcpReactor::epoller_ctl(conn_fd, EPOLL_CTL_ADD, EPOLLIN|EPOLLPRI);
							_handlers.insert(conn_fd, handler);
						}
						
					}
				}
				else
				{
					//处理普通输入事件
					EventHandlerPtr handler;
					if(_handlers.get(_ep_events[i].data.fd, handler))
					{
						nRet = handler->handle_input(_ep_events[i].data.fd);
						if(nRet != 0)
						{	
							printf("--- close success, fd:%d\n", _ep_events[i].data.fd);
							
							TcpReactor::epoller_ctl(_ep_events[i].data.fd, EPOLL_CTL_DEL, 0);
							handler->handle_close(_ep_events[i].data.fd);
							close(_ep_events[i].data.fd);
							_handlers.del(_ep_events[i].data.fd);
						}
					
					}
				}
				
			}
			else if(_ep_events[i].events & EPOLLPRI )
			{
				//处理带外输入事件
				EventHandlerPtr handler;
				if(_handlers.get(_ep_events[i].data.fd, handler))
				{
					handler->handle_exception(_ep_events[i].data.fd);	
				}
				
			}
			else
			{
				printf("other events is arrived.\n");
			}
			
		}
		
	}
	else if(fd_cnt == 0)
	{
		//printf("%llu: epoll_wait timeout, errno:%d, errmsg:%s\n", getTimestamp(), errno, strerror(errno));
	}
	else
	{
		if(errno == EINTR)
		{
			printf("epoll_wait is interrupt, errno:%d, errmsg:%s\n", errno, strerror(errno));
		}
		else
		{
			printf("epoll_wait failed, errno:%d, errmsg:%s\n", errno, strerror(errno));
			::abort();
			//return -1;
		} 
	}

	return 0;
	
}



int TcpReactor::epoller_create(int epoll_size)
{
	int nRet = 0;
	
	_epfd = epoll_create(epoll_size);
	if(_epfd == -1)
	{
		printf("epoll_create failed, errno:%d, errmsg:%s\n", errno, strerror(errno));
		return -1;
	}
	
	_epoll_size = epoll_size;
	_ep_events = new struct epoll_event[_epoll_size];
	
	return nRet;
}




int TcpReactor::epoller_ctl(int fd, int op, unsigned int events)
{
	int nRet = 0;

	struct epoll_event ep_ev;
	ep_ev.data.fd = fd;
	ep_ev.events = events;
	nRet = epoll_ctl(_epfd, op, fd, &ep_ev); 
	if(nRet != 0)
	{
		printf("epoll_ctl failed, fd:%d, errno:%d, errmsg:%s\n", fd, errno, strerror(errno));
	}

	return nRet;
	
}



int TcpReactor::listen_fd()
{
	return _listen_fd;
}



}


