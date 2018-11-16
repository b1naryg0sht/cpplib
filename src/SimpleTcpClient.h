#ifndef __CPPBRICK_SIMPLE_TCP_CLIENT_H__
#define __CPPBRICK_SIMPLE_TCP_CLIENT_H__

#include "StdTypes.h"

namespace cppbrick {

class SimpleTcpClient {
private:
	int sockfd;
public:
	SimpleTcpClient();
	~SimpleTcpClient();
	int Init();
	int Connect(const char* servIp, ushort_t servPort);
	int ReadData(char *buf,int recvlen);
	int WriteData(const char *buf, uint_t dataLen);
	void Disconnect();
	
	int InitTcpServer(ushort_t port);
	int Accept();
	int ReadData(int fd, char *buf,int recvlen);
	int WriteData(int fd, const char *buf, uint_t dataLen);	
};

}
#endif
