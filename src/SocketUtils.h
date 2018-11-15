#ifndef __CPPBRICK_SOCKET_UITLS_H__
#define __CPPBRICK_SOCKET_UITLS_H__

#include <string>

namespace cppbrick {

class SocketUtils {
public:
	/*
	在超时时间中循环接收指定长度的数据(同步阻塞方式)
	buf由外部应用申请空间
	len指定外部分配空间大小，处理后返回接收到的数据长度
	timeout单位是微秒
	返回值:
	(1) 0:fd关闭
	(2) 1:正常
	(3) 2:超时
	(4)小于0: fd接收失败返回负值错误码
	*/
	static int recv_n(int fd, char *buf, unsigned int &len, const unsigned int timeout=10000);



	/*
	在超时时间中仅接收一次指定长度的数据(同步阻塞方式)
	buf由外部应用申请空间
	len指定外部分配空间大小，处理后返回接收到的数据长度
	timeout单位是微秒
	返回值:
	(1) 0:fd关闭
	(2) 1:正常
	(3) 2:超时
	(4)小于0: fd接收失败返回负值错误码
	*/
	static int recv(int fd, char *buf, unsigned int &len, const unsigned int timeout=10000);
	


	/*
	在超时时间中循环发送指定长度的数据(同步阻塞方式)
	buf由外部应用申请空间
	len指定外部分配空间大小，处理后返回接收到的数据长度
	timeout单位是微秒
	返回值:
	(1) 1:正常
	(2) 2:超时
	(3)小于0: fd接收失败返回负值错误码
	*/
	static int send_n(int fd, const char *buf, unsigned int &len, int flags=0, const unsigned int timeout=10000);


	/*
	在超时时间中仅发送一次指定长度的数据(同步阻塞方式)
	buf由外部应用申请空间
	len指定外部分配空间大小，处理后返回接收到的数据长度
	timeout单位是微秒
	返回值:
	(1) 1:正常
	(2) 2:超时
	(3)小于0: fd接收失败返回负值错误码
	*/
	static int send(int fd, const char *buf, unsigned int &len, int flags=0, const unsigned int timeout=10000);


	static int send_s(int fd, const char *buf, unsigned int &len, int flags=0);
	


	/*
	(1) 1:正常
	(2) 2:超时
	(3)小于0: fd接收失败返回负值错误码
	*/
	static int connect(int fd, std::string ip, unsigned short port, const unsigned int timeout=0);


	static int connect_s(int fd, std::string ip, unsigned short port, const unsigned int timeout=0);	


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


