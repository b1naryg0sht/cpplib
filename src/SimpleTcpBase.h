#ifndef __CPPBRICK_SIMPLE_TCP_CLIENT_H__
#define __CPPBRICK_SIMPLE_TCP_CLIENT_H__

#include "StdTypes.h"

namespace cppbrick {

class SimpleTcpBase {
private:
	int sockfd;
public:
	SimpleTcpBase();
	~SimpleTcpBase();
	int InitTcpClient();
	int Connect(const char* servIp, ushort_t servPort);
	int ReadData(char *buf,int recvlen);
	int WriteData(const char *buf, uint_t dataLen);
	void Disconnect();
	
	int InitTcpServer(ushort_t port);
	int Accept();
	int ReadData(int fd, char *buf,int recvlen);
	int WriteData(int fd, const char *buf, uint_t dataLen);	

	// for client, return connect fd
	// for server, return accept fd
	int GetSockFd();
};

}
#endif
