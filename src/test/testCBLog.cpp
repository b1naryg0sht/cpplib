#include <iostream>
#include <unistd.h>
#include "CBLog.h"

int main()
{
	MY_LOG_DEBUG("logtest for debug");
	MY_LOG_DEBUG("logtest for debug port = %d, ip = %s", 8550, "192.168.1.113");
	MY_LOG_WARN("logtest for warn");
	MY_LOG_WARN("logtest for warn port = %d, ip = %s", 8550, "192.168.1.113");
	MY_LOG_ERROR("logtest for error");
	MY_LOG_ERROR("logtest for error port = %d, ip = %s", 8550, "192.168.1.113");
}
