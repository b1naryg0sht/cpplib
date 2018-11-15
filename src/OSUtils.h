#ifndef __CPPBRICK_OS_UTILS_H__
#define __CPPBRICK_OS_UTILS_H__

#include <pthread.h>

namespace cppbrick {

class OSUtils {
public:
	static std::string GetThreadId();
	static int get_rlimit(int resource, struct rlimit *rlim);
	static bool is_file_exist(const char *path);
	static long get_file_size(const char *path);
	static int signal_mask(int how, unsigned int num, ...);
};

}

#endif //__YOGURT__THREAD_UTILS_H__
