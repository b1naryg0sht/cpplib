#ifndef __CPPBRICK_CB_LOG_H__
#define __CPPBRICK_CB_LOG_H__

/*
1.日志输出的统一格式如下：
level|time|thread id|file@fucntion@line|user-defined message

1.1 level
日志的级别。
level显示的字符统一大写：DEBUG/WARNING/ERROR
日志至少要分3个层级:debug, warning, error.
debug:仅仅用于显示消息
warning:表示出现异常但不严重的情况
error:表示严重的错误

1.2 time
日志生成时间。格式为year-month-day hour-minute-second-usec

1.3 thread id
线程id，打印格式要用16进制，以便于多线程调试时查看。linux下，printf用 %lx控制16进制格式输出。

1.4 file@fucntion@line
日志的具体位置。用__FILE__, __func__, __LINE__来显示，注意用basename去掉文件在系统中的路径

1.5 user-defined message 
用户自定义的输出消息
*/

#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <pthread.h>
#include <libgen.h>


#define PrintTime     \
	struct timeval timep; \
    struct tm* p = NULL; \
    gettimeofday(&timep, NULL); \
    p = localtime((time_t*)&timep.tv_sec); \
    fprintf(stderr, "%4d/%02d/%02d %02d:%02d:%02d.%06u", p->tm_year+1900,p->tm_mon+1,p->tm_mday,p->tm_hour, p->tm_min,p->tm_sec, (unsigned int)timep.tv_usec);

#define CB_DEBUG_MODE

#ifdef CB_DEBUG_MODE

#define CB_FUNC_ENTRY fprintf(stderr, "%s entry @%d\r\n",__FUNCTION__, __LINE__)

#define MY_LOG_DEBUG(...) \
   do \
  { \
  	fprintf(stderr, "DEBUG|");\
  	PrintTime\
  	fprintf(stderr, "|%s@%s@%d|", basename(const_cast<char *>(__FILE__)), __func__, __LINE__); \
	fprintf(stderr, "0x%lx|", pthread_self()); \
  	fprintf(stderr, __VA_ARGS__); \
  	fprintf(stderr, "\n");\
  } \
  while(0)

#define MY_LOG_WARN(...) \
   do \
  { \
  	fprintf(stderr, "WARN|");\
  	PrintTime\
  	fprintf(stderr, "|%s@%s@%d|", basename(const_cast<char *>(__FILE__)), __func__, __LINE__); \
	fprintf(stderr, "0x%lx|", pthread_self()); \
  	fprintf(stderr, __VA_ARGS__); \
  	fprintf(stderr, "\n");\
  } \
  while(0)
            
#else
  
  #define MY_FUNC_ENTRY
  #define MY_LOG_DEBUG(...)
  #define MY_LOG_WARN(...)   
 
#endif


#define MY_LOG_ERROR(...) \
  do \
  { \
  	fprintf(stderr, "ERROR|");\
  	PrintTime\
  	fprintf(stderr, "|%s@%s@%d|", basename(const_cast<char *>(__FILE__)), __func__, __LINE__); \
	fprintf(stderr, "0x%lx|", pthread_self()); \
  	fprintf(stderr, __VA_ARGS__); \
  	fprintf(stderr, "\n");\
  } \
  while(0)

#endif