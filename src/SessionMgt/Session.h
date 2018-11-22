#ifndef __CPPBRICK_SESSION_H__
#define __CPPBRICK_SESSION_H__

#include <memory>

namespace cppbrick {

class Session
{
public:
	Session(): _id(""), _client_id(0), _fd(-1), _ip(""), _port(0), _status(false),
	_create_time(0), _access_time(0), _close_time(0), _r_num(0), _w_num(0)
	{

	}

	~Session(){}
	
public:
	std::string _id;
	unsigned long long _client_id;
	
	//std::string _uuid;  //安全通道ID 
	//std::string _key;   //安全通道秘钥
	
	int _fd;
	std::string _ip;
	unsigned short _port;
	bool _status;
	
	unsigned long long _create_time;  //微秒
	unsigned long long _access_time;
	unsigned long long _close_time;
	
	unsigned long long _r_num;
	unsigned long long _w_num;
		
};
typedef std::shared_ptr<Session>  SessionPtr;
}
#endif


