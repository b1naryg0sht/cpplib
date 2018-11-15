#ifndef __CPPBRICK_THREAD_MEET_H__
#define __CPPBRICK_THREAD_MEET_H__

#include <pthread.h>
#include "StdTypes.h"

/*
*这个类主要用于线程同步.同步的模型如下：
有三个线程A,B,C，他们需要分别在R点等到对方后才能继续执行。线程A,B,C需要分别在R点调用同一个ThreadMeet对象的Meet()接口。
|	|	|
|	|	|
A	B	C
|	|	|
|	|	|
\	|	/
 \	|  /
  \ | /
    R
    |
    |
*/

namespace cppbrick {

class ThreadMeet {
private:
  uint_t count;
  uint_t maxCount;
  pthread_mutex_t lock;
  pthread_cond_t  cond;
public:
	ThreadMeet(uint_t maxCount);
	~ThreadMeet();
	//@timeout:在R点最多等待时间,单位us.0表示永远等待
	void Meet(uint_t timeout=0);
	void Reset();
	void SetMaxcount(uint_t maxCount);
} ;

}
#endif



