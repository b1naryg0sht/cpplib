#include <iostream>
#include <unistd.h>
#include "Conn.h"
#include "ConnMgtLB.h"
#include "TcpClientEpoll.h"

using namespace cppbrick;


int main()
{
	std::vector<StSvr> ls;
	StSvr svr;
	svr._ip = "127.0.0.1";
	svr._port = 17958;
	ls.push_back(svr);
	
	ConnMgtLB conn_mgt;
	conn_mgt.init<TcpClientEpoll>(ls);

	sleep(100);
	return 0;
}
