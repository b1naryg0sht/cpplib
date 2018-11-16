#include<stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include "SimpleTcpClient.h"
#include "Log.h"

namespace cppbrick {


SimpleTcpClient::SimpleTcpClient():sockfd(-1)
{
	
}

SimpleTcpClient::~SimpleTcpClient()
{
	
}

int SimpleTcpClient::Init()
{
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if (sockfd == -1)
	{
		CB_LOG_ERROR("create sock fail .\n");
		return -1;
	}
	
	int opt = 1;
	if (-1 == setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(int)))
	{
		close(sockfd);
		sockfd = -1;
		CB_LOG_ERROR("setsockopt fail .\n");

		return -1;
	}	
	return 0;	
}


int SimpleTcpClient::InitTcpServer(ushort_t localPort)
{
    int fd;
    struct sockaddr_in server_addr; 

    fd = socket(AF_INET,SOCK_STREAM,0);
    if(-1 == fd)
    {
        CB_LOG_ERROR("create sock fail .\n");
        return -1;
    }

    bzero(&server_addr,sizeof(server_addr));
    server_addr.sin_family      = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(localPort);

    int opt = 1;
    if (-1 == setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(int)))
    {
        close(fd);
        fd = -1;
        CB_LOG_ERROR("setsockopt fail .\n");

        return -1;
    }

    if(-1 == bind(fd,(struct sockaddr *)&server_addr,sizeof(server_addr)))
    {
        close(fd);
        fd = -1;
        CB_LOG_ERROR(" bind sock fail .\n");
        return -1;
    }
    listen(fd,10);
	CB_LOG_DEBUG("listenfd:%d\n", fd);
	sockfd = fd;
    return 0;
}

int SimpleTcpClient::Accept()
{
	if(sockfd < 0)
	{
		CB_LOG_ERROR("Accept fail. sockfd <0\n");
		return -1;
	}
	
    int client_sock;
    struct sockaddr_in clientAddr; 
    int addr_len = sizeof(clientAddr); 

    client_sock = accept(sockfd,(struct sockaddr *)&clientAddr,(socklen_t*)&addr_len);
    if(-1 == client_sock)
    {
        CB_LOG_ERROR("tcp accept fail.sockfd:%d, client_sock:%d, error:%s\n", sockfd, client_sock, strerror(errno));
        return -1;
    }

    return client_sock;
}

int SimpleTcpClient::Connect(const char* servIp, ushort_t servPort)
{
	struct sockaddr_in _serverAddr;

	if( sockfd < 0)
	{
		return -1;
	}

	_serverAddr.sin_family = AF_INET;
	_serverAddr.sin_port = htons(servPort);
	_serverAddr.sin_addr.s_addr = inet_addr(servIp);
	memset(_serverAddr.sin_zero, 0, 8);
	if(-1 == connect(sockfd, (struct sockaddr*)&_serverAddr, sizeof(struct sockaddr)))
	{	
		CB_LOG_ERROR("connect sock error=%d.\n",errno);
		return -1;
	}
	
	return 0;
}

int SimpleTcpClient::ReadData(char *buf,int recvlen)
{  
	if(sockfd < 0)
	{
		CB_LOG_ERROR("ReadData fail. sockfd <0\n");
		return -1;
	}

    int len;

    len=recv(sockfd,buf,recvlen,0);

    return len;
}

int SimpleTcpClient::ReadData(int fd, char *buf,int recvlen)
{  
	if(fd < 0)
	{	
		CB_LOG_ERROR("ReadData fail. fd <0\n");
		return -1;
	}

    int len;

    len=recv(fd,buf,recvlen,0);

    return len;
}


int SimpleTcpClient::WriteData(const char *buf, uint_t dataLen)
{
	int rs=0;
	int leastLen=dataLen;
	int sendLen=0;
	
	if(sockfd < 0)
	{	
		CB_LOG_ERROR("WriteData fail. sockfd <0\n");
		return -1;
	}

	
	if(buf==NULL)
	{
		CB_LOG_ERROR("WriteData fail. buf==NULL\n");
		return -2;
	}	
	while(leastLen)
	{
		rs=send(sockfd,(void*)(buf+sendLen),leastLen,0);
		if(rs<=0)
		{
			CB_LOG_ERROR("send data  to net error.\n");
			return -2;
		}
		sendLen+=rs;
		leastLen=leastLen-sendLen;
	}
	
	return sendLen;
}

int SimpleTcpClient::WriteData(int fd, const char *buf, uint_t dataLen)
{
	int rs=0;
	int leastLen=dataLen;
	int sendLen=0;
	
	if(fd < 0)
	{	
		CB_LOG_ERROR("WriteData fail. fd <0\n");
		return -1;
	}

	
	if(buf==NULL)
	{
		CB_LOG_ERROR("WriteData fail. buf==NULL\n");
		return -2;
	}	
	while(leastLen)
	{
		rs=send(fd,(void*)(buf+sendLen),leastLen,0);
		if(rs<=0)
		{
			CB_LOG_ERROR("send data  to net error.\n");
			return -2;
		}
		sendLen+=rs;
		leastLen=leastLen-sendLen;
	}
	
	return sendLen;
}


void SimpleTcpClient::Disconnect()
{	
	if (sockfd > 0)
	{
		close(sockfd);
	}
}

}
