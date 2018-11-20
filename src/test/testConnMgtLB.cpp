#include <iostream>
#include <unistd.h>
#include "Conn.h"
#include "ConnMgtLB.h"
#include "TcpClientEpoll.h"

using namespace cppbrick;


int main()
{
#if 1
	std::vector<StSvr> ls;
	StSvr svr;
	svr._ip = "127.0.0.1";
	svr._port = 17958;
	ls.push_back(svr);
#endif	
	ConnMgtLB conn_mgt;
	conn_mgt.init<TCP_Client_Epoll>(ls);

	sleep(100);
	return 0;
}
