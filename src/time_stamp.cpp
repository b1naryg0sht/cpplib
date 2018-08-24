#include <time_stamp.h>
#include <cstring>
#include <sys/time.h>


namespace Yogurt {

    TimeStamp TimeStamp::_startStamp;

    TimeStamp::TimeStamp() 
	{
        struct timeval tv;
        gettimeofday(&tv, NULL);
        _seconds = tv.tv_sec;
        _microSeconds = tv.tv_usec;
    }

	TimeStamp &TimeStamp::operator=(const TimeStamp &other)
	{
		_seconds = other._seconds;
		_microSeconds = other._microSeconds;
		return *this;
	}

    TimeStamp::TimeStamp(unsigned long seconds, unsigned long microSeconds) :
        _seconds(seconds),
        _microSeconds(microSeconds) 
	{
    }
	
	std::string TimeStamp::getFormatTimeStamp() const
	{	
		std::string strDateTime = "";

		setenv("TZ", "GMT-8", 1);
		tzset();
		
		struct tm *ptm = NULL;
		struct tm ret_tm = {0};
		ptm = localtime_r((time_t*)&(_seconds), &ret_tm);
		if(ptm == NULL)
		{
			printf("localtime_r failed. errno:%d, errmsg:%s\n", errno, strerror(errno));	
			return strDateTime;
		}
#if 1
		char tmp[32]={0};
		snprintf(tmp, sizeof(tmp), "%04d%02d%02d %02d%02d%02d.%lu", ret_tm.tm_year+1900, ret_tm.tm_mon+1, ret_tm.tm_mday,
			ret_tm.tm_hour, ret_tm.tm_min, ret_tm.tm_sec, _microSeconds);
#else
		char tmp[100] = {0};
		strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S %Z", &ret_tm);
#endif
		strDateTime.assign(tmp);
		return strDateTime;
	}
}


