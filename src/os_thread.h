#ifndef __YOGURT_OSTHREAD_H__
#define __YOGURT_OSTHREAD_H__

#include <pthread.h>
#include <vector>

namespace Yogurt {


class OSThread
{
public:
	enum StStatus
	{
		ST_INIT,
		ST_RUN,
		ST_EXIT,
	};

	virtual int prepare(){return 0;};
	virtual int do_init(void *args);	
	virtual int svc() = 0;

	virtual void cleanup(){pthread_attr_destroy(&_attr);};
	
public:
	OSThread();
	
	virtual ~OSThread();


	int init(void *args=NULL, unsigned int thr_cnt=1, bool detach=true, unsigned int stack_size=0);


	int run(void **ret=NULL);

    int exit(void *ret=NULL);
	
	static pthread_t thread_id();
	
	void stop();

	bool is_stop();

	int thr_count();

	void thr_status(int status);

	int thr_status();
	
private:
	int join(pthread_t thread, void **ret);

	int detach(pthread_t thread);

private:
    pthread_attr_t _attr;
	unsigned int _thr_cnt;

	int _status;
	int _suspend;
	int _stop;
	
	bool _detach;
	unsigned int _stack_size;

	std::vector<pthread_t> _threads;
};

}// namespace Yogurt
#endif //__YOGURT_OSTHREAD_H__

