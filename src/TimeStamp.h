#ifndef __CPPBRICK_TIMESTAMP_H__
#define __CPPBRICK_TIMESTAMP_H__

#include <string>

namespace cppbrick {

    class TimeStamp 
    {
        public:
        /**
           Constructs a TimeStamp representing 'now'.
        **/
        TimeStamp();
		TimeStamp &operator=(const TimeStamp &other);
		TimeStamp operator-(const TimeStamp &other);
		bool operator>(const TimeStamp &other);
		bool operator<(const TimeStamp &other);

        TimeStamp(unsigned long seconds, unsigned long microSeconds = 0);

        inline long getSeconds() const 
		{
            return _seconds;
        };

		//msec
        inline long getMSeconds() const 
		{
            return _microSeconds / 1000;
        };

		//usec
        inline long getUSeconds() const 
		{
            return _microSeconds;
        };

		inline void setSeconds(const long val)
		{
			_seconds = val;
		}

		inline void setUSeconds(const long val)
		{
			_microSeconds = val;
		}

		
		inline long ToUSeconds() const
		{
			return _seconds*1000000 + _microSeconds;
		}

		inline long ToMSeconds() const
		{
			return _seconds*1000 + _microSeconds/1000;
		}		
		std::string getFormatTimeStamp() const;	

        /**
           Returns a TimeStamp representing the time at which the application
           started.
        **/
        static inline const TimeStamp& getStartTime() 
		{
            return _startStamp;
        };

        protected:
        static TimeStamp _startStamp;

        long _seconds;
        long _microSeconds;
    };
}

#endif

