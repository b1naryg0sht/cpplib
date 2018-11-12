#include <iostream>
#include <unistd.h>
#include <vector>
#include <assert.h>
#include <iostream>
#include <StdTypes.h>
#include <StringUtils.h>


using namespace cppbrick;

int main()
{
	std::vector<std::string> els;
	std::string istr = "1|2|3|4";
	StringUtils::split(els,istr,'|');
	for(uint_t i=0;i<els.size();i++)
	{
		std::cout << els[i] << std::endl;
	}

	istr = "	1234";
	assert(StringUtils::trim(istr) == "1234");
	istr = " 1234";
	assert(StringUtils::trim(istr) == "1234");
	istr = "1234 ";
	assert(StringUtils::trim(istr) == "1234");
	istr = "1234 ";
	assert(StringUtils::trim(istr) == "1234");
	istr = "	1234 ";
	assert(StringUtils::trim(istr) == "1234");
	istr = "	12 34 ";
	assert(StringUtils::trim(istr) == "12 34");
	istr = "1234";
	assert(StringUtils::trim(istr) == "1234");

	
	assert(StringUtils::format("hello, world") == "hello, world");
	assert(StringUtils::format("hello, %s", "world") == "hello, world");
	assert(StringUtils::format("%s, %s", "hello", "world") == "hello, world");

}

