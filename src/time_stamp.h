#ifndef __YOGURT_TIMESTAMP_H__
#define __YOGURT_TIMESTAMP_H__

#include <string>

namespace Yogurt {

    class TimeStamp 
    {
        public:
        /**
           Constructs a TimeStamp representing 'now'.
        **/
        TimeStamp();
		TimeStamp &operator=(const TimeStamp &other);

        TimeStamp(unsigned long seconds, unsigned long microSeconds = 0);

        inline long getSeconds() const 
		{
            return _seconds;
        };

        inline long getMilliSeconds() const 
		{
            return _microSeconds / 1000;
        };

        inline long getMicroSeconds() const 
		{
            return _microSeconds;
        };

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

#endif // __YOGURT_TIMESTAMP_H__

