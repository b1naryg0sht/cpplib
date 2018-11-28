#include <iostream>
#include <unistd.h>
#include "SimpleTcpBase.h"
#include "Log.h"

using namespace cppbrick;

int main()
{
	SimpleTcpBase client;
	int ret = client.InitTcpClient();
	if(ret < 0)
	{
		CB_LOG_ERROR("tcp client init fail."); 
		return -1;
	}

	const char *svr_ip = "127.0.0.1";
	ushort_t port = 10086;
	ret = client.Connect("127.0.0.1", 17958);
	if(ret < 0)
	{
		CB_LOG_ERROR("tcp client connect to %s:%d fail.", svr_ip, port); 
		return -1;
	}	

	std::string data = "what's your name?";
	client.WriteData(data.c_str(), data.size());

	char buf[1024] = {0};
	client.ReadData(buf, 1024);
	CB_LOG_DEBUG("recv from server:%s", buf);

	client.Disconnect();
	
	return 0;
}

