#ifndef __CPPBRICK_BASE64_H__
#define __CPPBRICK_BASE64_H__

namespace cppbrick {

//--------------------------- base64 ------------------------------
/*
Base64就是用来将非ASCII字符的数据转换成ASCII字符的一种方法。
用于某些系统中只能使用ASCII字符系统。
base64特别适合在http，mime协议下快速传输数据。
目前base64在下载地址上得到广泛的使用。
*/
class Base64
{
public:

	static int encrypt(const char *pSrc, const unsigned int len, char* &pDst);

	static int decrypt(const char *pSrc, char* &pDst);
	
};

}


#endif //__CPPBRICK_BASE64_H__


