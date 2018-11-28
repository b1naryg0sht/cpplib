#include "ConnMgtLB.h"
#include "StringUtils.h"
#include "Log.h"

namespace cppbrick {


ConnMgtLB::ConnMgtLB()
{

}



ConnMgtLB::~ConnMgtLB()
{

}


void ConnMgtLB::conn_timer_cb()
{
	check_conn();
}




void ConnMgtLB::check_conn()
{
	ThreadMutexGuard guard(_mutex);
	
	std::vector<ConnPtr>::iterator itr = _conns.begin();
	for(; itr != _conns.end(); itr++)
	{
		(*itr)->connect();
	}
}



bool ConnMgtLB::get_conn(ConnPtr conn)
{
	ThreadMutexGuard guard(_mutex);

	if(_conns.empty())
	{
		CB_LOG_ERROR( "conn is empty.");
		return false;
	}

	if(_index >= _conns.size())
	{
		_index = 0;
	}
	
	unsigned int tag = _index;
	do
	{
		conn = _conns[_index];
		++_index;
		if(!conn->is_close())
		{
			/*CB_LOG_ERROR( "get (%s:%u) conn success, index:%u, total:%u\n", 
				conn->_svr._ip.c_str(), conn->_svr._port, (_index-1), _conns.size());*/
			return true;
		}
		else
		{
			CB_LOG_ERROR( "the conn (%s:%u) is closed, index:%u, total:%lu", 
				conn->_svr._ip.c_str(), conn->_svr._port, (_index-1), _conns.size());
		}

		if(_index >= _conns.size())
		{
			_index = 0;
		}

	}while(_index != tag);

	CB_LOG_ERROR( "no conn is found.");
	return false;

}

}

