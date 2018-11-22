#include "SessionMgt.h"
#include "StringUtils.h"
#include "TimeStamp.h"
#include "SocketUtils.h"
#include "Log.h"

namespace cppbrick {

const ulong_t TCP_IDLE_TIMEOUT = 60000000UL;

SessionMgt::SessionMgt():m_uid(0)
{
	InitSessionTimer();
}

SessionMgt::~SessionMgt()
{

}

int SessionMgt::InitSessionTimer()
{
	SelectTimer *timer_session = new SelectTimer;
	SessionTimerHandler *session_thandler = new SessionTimerHandler;
	int nRet = timer_session->register_timer_handler(session_thandler, TCP_IDLE_TIMEOUT);
	if(nRet != 0)
	{
		CB_LOG_ERROR("register session timer handler falied, ret:%d", nRet);
		return -1;
	}
		
	nRet = timer_session->init();
	if(nRet != 0)
	{
		CB_LOG_ERROR("session timer init failed, ret:%d", nRet);
		return -1;
	}

	nRet = timer_session->run();
	if(nRet != 0)
	{
		CB_LOG_ERROR("session timer run failed, ret:%d", nRet);
		return -1;
	}
	return 0;
}

std::string SessionMgt::gen_uid(std::string &prefix, ulong_t second)
{
	m_uid++;
	// prefix_timestamp_uid
	return prefix+std::string("_")+std::to_string(second)+std::string("_")+std::to_string(m_uid);
}


void SessionMgt::insert_session(int fd, std::string &id)
{
	ThreadMutexGuard guard(_mutex);

	//获取客户端ip 和port
	std::string ip = "";
	unsigned short port = 0; 
	SocketUtils::get_remote_socket(fd, ip, port);

	TimeStamp ts;
	//获取session id
	std::string tmp = StringUtils::format("%s_%u", ip.c_str(), port);
	id = gen_uid(tmp, ts.getSeconds());

	Session session;
	session._id = id;
	session._fd = fd;
	session._status = true;
	session._ip = ip;
	session._port = port;
	session._create_time = ts.ToUSeconds();
	session._access_time = ts.ToUSeconds();
	_sessions[fd] = session;
	_fds[id] = fd;

}



void SessionMgt::remove_session(int fd, std::string &id)
{
	ThreadMutexGuard guard(_mutex);

	std::map<int, Session>::iterator itr = _sessions.find(fd);
	if(itr != _sessions.end())
	{
		id = itr->second._id;

		TimeStamp ts;

		itr->second._close_time = ts.ToUSeconds();
		CB_LOG_INFO("remove session(id:%s, fd:%d)", id.c_str(), fd);
		
		_fds.erase(itr->second._id);
		_sessions.erase(itr);
		
	}
}


bool SessionMgt::is_valid_sesssion(const std::string &id)
{
	ThreadMutexGuard guard(_mutex);

	std::map<std::string, int>::iterator itr = _fds.find(id);
	if(itr != _fds.end())
	{
		return true;
	}

	return false;
}



bool SessionMgt::get_fd(const std::string &id, int &fd)
{
	ThreadMutexGuard guard(_mutex);

	std::map<std::string, int>::iterator itr = _fds.find(id);
	if(itr != _fds.end())
	{
		fd = itr->second;
		return true;
	}

	return false;
}


void SessionMgt::update_status(int fd, bool status)
{
	ThreadMutexGuard guard(_mutex);

	std::map<int, Session>::iterator itr = _sessions.find(fd);
	if(itr != _sessions.end())
	{
		itr->second._status = status;
	}
	else
	{
		CB_LOG_INFO("fd(%d) isn't found in _sessions.", fd);
	}
}



void SessionMgt::update_read(int fd, unsigned long long num, std::string &id)
{
	ThreadMutexGuard guard(_mutex);

	std::map<int, Session>::iterator itr = _sessions.find(fd);
	if(itr != _sessions.end())
	{
		if(itr->second._status)
		{
			TimeStamp ts;
			itr->second._access_time = ts.ToUSeconds();
			itr->second._r_num += num;
			id = itr->second._id;
		}
	}
}



void SessionMgt::update_write(int fd, unsigned long long num)
{
	ThreadMutexGuard guard(_mutex);

	std::map<int, Session>::iterator itr = _sessions.find(fd);
	if(itr != _sessions.end())
	{
		if(itr->second._status)
		{
			TimeStamp ts;
			itr->second._access_time = ts.ToUSeconds();
			itr->second._w_num += num;
		}
	}
}




bool SessionMgt::set_client_id(const std::string &id, const unsigned long long client_id)
{
	ThreadMutexGuard guard(_mutex);

	std::map<std::string, int>::iterator itr = _fds.find(id);
	if(itr == _fds.end())
	{
		CB_LOG_INFO("session(%s) isn't exist.", id.c_str());
		return false;
	}

	std::map<int, Session>::iterator itr1 = _sessions.find(itr->second);
	if(itr1 != _sessions.end())
	{
		itr1->second._client_id = client_id;
	}

	return true;
}




bool SessionMgt::get_client_id(const std::string &id, unsigned long long &client_id)
{
	ThreadMutexGuard guard(_mutex);

	std::map<std::string, int>::iterator itr = _fds.find(id);
	if(itr == _fds.end())
	{
		CB_LOG_INFO("session(%s) isn't exist.", id.c_str());
		return false;
	}

	std::map<int, Session>::iterator itr1 = _sessions.find(itr->second);
	if(itr1 != _sessions.end())
	{
		client_id = itr1->second._client_id;
	}

	return true;

}




/*
【重要】检查的会话最好不要超过5W个
要保证在3分钟中很轻松就把5W会话检查完成
*/
void SessionMgt::check()
{
	//CB_FUNC_ENTRY;
	
	ThreadMutexGuard guard(_mutex);

	std::string err_info = "";
		
	//计算当前时间戳
	TimeStamp curts;
	unsigned long long cur_stmp = curts.ToUSeconds();
	std::map<int, Session>::iterator itr = _sessions.begin();
	for(; itr != _sessions.end();)
	{
		//会话超过60(默认)秒没有接收任何数据，Server自动关闭该连接
		if(cur_stmp - (itr->second._access_time) > TCP_IDLE_TIMEOUT)
		{
			int fd = itr->first;

			std::string local_ip = "";
			unsigned short local_port = 0;
			SocketUtils::get_local_socket(fd, local_ip, local_port);
			
			std::string remote_ip = "";
			unsigned short remote_port = 0; 
			SocketUtils::get_remote_socket(fd, remote_ip, remote_port);
			
			CB_LOG_INFO("close to app(fd:%d), %s:%u --> %s:%u", 
				fd, local_ip.c_str(), local_port, remote_ip.c_str(), remote_port);

			TimeStamp now;
			itr->second._close_time = now.ToUSeconds();
			CB_LOG_INFO("===timeout: the session is timeout, prepare to close it, access_time:%llu, cur_stmp:%llu", (itr->second._access_time), cur_stmp);

			std::string session_id = itr->second._id;
			_fds.erase(session_id);
			_sessions.erase(itr++);
			// TODO:remove fd in tcp reactor
			
		}
		else
		{
			++itr;
		}

		
	}

}

}

