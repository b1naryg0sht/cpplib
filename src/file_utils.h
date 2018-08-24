#ifndef __YOGURT_FILE_UTILS_H__
#define __YOGURT_FILE_UTILS_H__

class FileUtils {
public:
	static bool is_file_exist(const char *path);
	static long get_file_size(const char *path);
};

#endif //__YOGURT_FILE_UTILS_H__
