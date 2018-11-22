#ifndef __CPPBRICK_SessionMgt_H__
#define __CPPBRICK_SessionMgt_H__

#include <map>
#include <iostream>
#include "TSingleton.h"
#include "ThreadMutex.h"
#include "Session.h"
#include "StdTypes.h"
#include "SelectTimer.h"

namespace cppbrick {

/*
SessionMgt的基本职责：维护tcp连接池
1.检测连接，如果60s内没有数据交互，则关闭连接
2.连接的增/删/查
3.更新连接的状态
SessionMgt需要配合前段的TcpReactor使用，在TcpReactor的EventHandler中的handle_accept中调用insert_session，在handle_close中调用remove_session。
而用户需要在check函数中调用TcpReactor实例来移除其中的fd
*/
class SessionMgt
{
public:
	SessionMgt();

	~SessionMgt();

	void insert_session(int fd, std::string &id);
	void remove_session(int fd, std::string &id);

	bool is_valid_sesssion(const std::string &id);
	bool get_fd(const std::string &sid, int &fd);

	void update_status(int fd, bool status);
	void update_read(int fd, unsigned long long num, std::string &id);
	void update_write(int fd, unsigned long long num);

	//client id 管理
	bool set_client_id(const std::string &id, const unsigned long long client_id);
	bool get_client_id(const std::string &id, unsigned long long &client_id);

	void check();

private:
	std::string gen_uid(std::string &prefix, ulong_t second);
	int InitSessionTimer();
private:
	ThreadMutex _mutex;
	//fd --- session
	std::map<int, Session> _sessions;
	//session id --- fd
	std::map<std::string, int> _fds;
	
	unsigned long long m_uid;
};

#define PSGT_SessionMgt TSingleton<SessionMgt>::GetInstance()

class SessionTimerHandler : public SelectTimerHandler
{
public:
	SessionTimerHandler() {}

	virtual ~SessionTimerHandler(){}

	virtual int handle_timeout(void *args = NULL)
	{
		PSGT_SessionMgt->check();	
		return 0;		
	}
};


}

#endif


