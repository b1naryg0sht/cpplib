#include <iterator>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "StringUtils.h"

namespace cppbrick {

std::string StringUtils::vform(const char* format, va_list args) 
{
    size_t size = 1024;
    char* buffer = new char[size];
        
    while (1) 
    {
        va_list args_copy;
        va_copy(args_copy, args);

        int n = vsnprintf(buffer, size, format, args_copy);

        va_end(args_copy);
            
        // If that worked, return a string.
        if ((n > -1) && (static_cast<size_t>(n) < size)) 
        {
            std::string s(buffer);
            delete [] buffer;
            return s;
        }
            
        // Else try again with more space.
        size = (n > -1) ?
            n + 1 :   // ISO/IEC 9899:1999
            size * 2; // twice the old size
            
        delete [] buffer;
        buffer = new char[size];
    }
}

std::string StringUtils::format(const char* fmt, ...) 
{
	va_list va;
	va_start(va,fmt);
	std::string s=vform(fmt, va);
	va_end(va);
	return s;
}
std::string StringUtils::trim(const std::string& s) 
{
    static const char* whiteSpace = " \t\r\n";

    // test for null string
    if(s.empty())
        return s;

    // find first non-space character
    std::string::size_type b = s.find_first_not_of(whiteSpace);
    if(b == std::string::npos) // No non-spaces
        return "";

    // find last non-space character
    std::string::size_type e = s.find_last_not_of(whiteSpace);

    // return the remaining characters
    return std::string(s, b, e - b + 1);
}

unsigned int StringUtils::split(std::vector<std::string>& v,
                               const std::string& s,
                               char delimiter, unsigned int maxSegments) 
{
    v.clear();
    std::back_insert_iterator<std::vector<std::string> > it(v);
    return split(it, s, delimiter, maxSegments);
}

void StringUtils::split(const std::string str, const std::string delim, std::vector<std::string> &vecStr, int num)
{
   if(str.empty() || delim.empty())
   {
	   return;
   }
   
   char *pBuffer = new char[str.size()+1];
   memcpy(pBuffer, str.c_str(), str.size());
   pBuffer[str.size()] = '\0';

   int _num = 0;
   char *pToken = NULL;
   char *pSaveptr = NULL;
   pToken = strtok_r(pBuffer, delim.c_str(), &pSaveptr);
   while(pToken != NULL)
   {
	   ++_num;
	   vecStr.push_back(pToken);
	   pToken = strtok_r(NULL, delim.c_str(), &pSaveptr);
	   if(_num == num)
	   {
		   break;
	   }
   }

   delete [] pBuffer;
   
};

void StringUtils::split(const std::string str, const std::string delim, std::list<std::string> &lstStr, int num)
{

	if(str.empty() || delim.empty())
	{
		return;
	}
	
	char *pBuffer = new char[str.size()+1];
	memcpy(pBuffer, str.c_str(), str.size());
	pBuffer[str.size()] = '\0';

	int _num = 0;
	char *pToken = NULL;
	char *pSaveptr = NULL;
	pToken = strtok_r(pBuffer, delim.c_str(), &pSaveptr);
	while(pToken != NULL)
	{
		++_num;
		
		lstStr.push_back(pToken);
		pToken = strtok_r(NULL, delim.c_str(), &pSaveptr);
		if(_num == num)
		{
			break;
		}
	}

	delete [] pBuffer;
}

}// namespace cppbrick

