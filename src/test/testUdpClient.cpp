#include <iostream>
#include <Thread.h>
#include <unistd.h>
#include <UdpClient.h>
#include <string.h>

using namespace cppbrick;


using namespace cppbrick;

int main()
{
	UdpClient client;
	std::string ip="127.0.0.1";
	ushort_t port = 10086;
	client.connect(ip, port, false);
	char *msg = "hello, world";
	client.sendto(msg, strlen(msg));
	client.close();
	return 0;
}
