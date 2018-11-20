#ifndef __CPPBRICK_CONN_MGT_LB_H__
#define __CPPBRICK_CONN_MGT_LB_H__

#include "SelectTimer.h"
#include "ThreadMutex.h"
#include "Conn.h"

#include "Log.h"
#include "StringUtils.h"


namespace cppbrick {

class ConnMgtLB
{
public:
	ConnMgtLB();

	virtual ~ConnMgtLB();
	
	class ConnTimerHandler : public SelectTimerHandler
	{
	public:
		ConnTimerHandler(ConnMgtLB *conn_mgt):_conn_mgt(conn_mgt) {}
		virtual int handle_timeout(void *args = NULL)
		{
			_conn_mgt->conn_timer_cb();
			return 0;
		}
		
	public:
		ConnMgtLB *_conn_mgt;
	};


	template <typename T>
	int init(const std::vector<StSvr> &svrs)
	{
		int nRet = 0;
		
		_index = 0;
		
		refresh<T>(svrs);
		
		SelectTimer *timer_req = new SelectTimer;
		nRet = timer_req->register_timer_handler(new ConnTimerHandler(this), 5000000);
		if(nRet != 0)
		{
			CB_LOG_ERROR( "register conn mgt lb timer handler falied, ret:%d\n", nRet);
			return nRet;
		}
			
		nRet = timer_req->init();
		if(nRet != 0)
		{
			CB_LOG_ERROR( "int conn mgt lb timer failed, ret:%d\n", nRet);
			return nRet;
		}
		
		nRet = timer_req->run();
		if(nRet != 0)
		{
			CB_LOG_ERROR( "conn mgt lb timer run failed, ret:%d\n", nRet);
			return nRet;
		}
		CB_LOG_DEBUG( "=== complete to start conn mgt lb timer ===\n");
		
		return 0;

	}
	
	template <typename T>
	void refresh(const std::vector<StSvr> &svrs)
	{
		ThreadMutexGuard guard(_mutex);
		
		std::vector<ConnPtr>::iterator itr = _conns.begin();
		for(; itr != _conns.end();)
		{
			bool found = false;
			for(unsigned int i=0; i<svrs.size(); i++)
			{
				if((svrs[i]._ip == (*itr)->_svr._ip) && (svrs[i]._port == (*itr)->_svr._port))
				{
					found = true;
					break;
				}
			}
		
			if(!found)
			{
				std::string id = StringUtils::format("%s_%u", (*itr)->_svr._ip.c_str(), (*itr)->_svr._port);
				CB_LOG_ERROR( "**** release useless conn(%s) ****\n", id.c_str());
				printf("**** release useless conn(%s) ****\n", id.c_str());
				
				(*itr)->close();
				itr = _conns.erase(itr); //����Conn��������
			}
			else
			{
				++itr;
			}
		
		}
		
			
		for(unsigned int i=0; i<svrs.size(); i++)
		{
			bool found = false;
			std::vector<ConnPtr>::iterator itr = _conns.begin();
			for(; itr != _conns.end(); itr++)
			{
				if((svrs[i]._ip == (*itr)->_svr._ip) && (svrs[i]._port == (*itr)->_svr._port))
				{
					found = true;
					break;
				}
			
			}
		
			if(!found)
			{
				std::string id = StringUtils::format("%s_%u", svrs[i]._ip.c_str(), svrs[i]._port);
				CB_LOG_ERROR( "**** add new conn(%s) ****\n", id.c_str());
				printf("**** add new conn(%s) ****\n", id.c_str());
				
				ConnPtr conn = std::make_shared<T>(svrs[i]);
				conn->connect();
				
				_conns.push_back(conn);
			}
			
		}

	}

	void conn_timer_cb();

	// send
	
private:

	void check_conn();
	bool get_conn(ConnPtr conn);
	
	ThreadMutex _mutex;
	std::vector<ConnPtr> _conns;
	unsigned int _index;	
};




}
#endif

