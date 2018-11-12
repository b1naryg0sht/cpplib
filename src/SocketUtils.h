#ifndef __CPPBRICK_SOCKET_UITLS_H__
#define __CPPBRICK_SOCKET_UITLS_H__

#include <string>
#include <errno.h>


namespace cppbrick {

class SocketUitls {
public:
	//设置socket 为非阻塞
	static int set_non_bolck(int fd);

	//设置socket 为 阻塞
	static int set_bolck(int fd);

	static int set_nodelay(int fd);
		
	//TCP状态位于 TIME_WAIT ，可以重用端口
	static int set_reuseaddr(int fd);

	static int set_keepalive(int fd);

	static int set_closeonexec(int fd);

	static int set_rcvbuf(int fd, int size);

	static int set_sndbuf(int fd, int size);

	static int set_quickack(int fd);

	static int set_rcv_timeout(int fd, int timeout);

	static int set_snd_timeout(int fd, int timeout);	

	static int shutdown(int fd, int flag);

	static int close_socket(int fd);

	static int get_local_socket(int fd, std::string &ip, unsigned short &port);
	
	static int get_remote_socket(int fd, std::string &ip, unsigned short &port);
};

}


#endif


