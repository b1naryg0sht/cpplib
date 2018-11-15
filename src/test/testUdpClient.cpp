#include <iostream>
#include <unistd.h>
#include <string.h>
#include "Thread.h"
#include "UdpClient.h"

using namespace cppbrick;

int main()
{
	UdpClient client;
	std::string ip="127.0.0.1";
	ushort_t port = 17958;
	client.connect(ip, port, false);
	//char *msg = "hello, world";
	client.sendto(NULL, 0);
	client.close();
	return 0;
}
