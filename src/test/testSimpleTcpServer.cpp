#include <iostream>
#include <unistd.h>
#include "SimpleTcpClient.h"
#include "Log.h"

using namespace cppbrick;

int main()
{
	SimpleTcpClient Server;
	int ret = Server.InitTcpServer(10086);
	if(ret < 0)
	{
		CB_LOG_ERROR("tcp client server fail."); 
		return -1;
	}

	while(1)
	{
		int clientfd = Server.Accept();
		if(clientfd < 0)
		{
			CB_LOG_ERROR("accept fails.");
			exit(1);
		}
		char buf[1024] = {0};
		Server.ReadData(clientfd, buf, 1024);
		CB_LOG_DEBUG("recv from client:%s", buf);
		std::string data = "nobody";
		Server.WriteData(clientfd, data.c_str(), data.size());
		close(clientfd);
	}
	
	return 0;
}

