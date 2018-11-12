#ifndef __CPPBRICK_md5_H__
#define __CPPBRICK_md5_H__

namespace cppbrick {

//可以指定size，如果不指定api 内部会通过strlen计算size
std::string md5(const char *str, unsigned int size=0);

}

#endif //__CPPBRICK_md5_H__

