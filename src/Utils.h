#ifndef __CPPBRICK_UTILS_H__
#define __CPPBRICK_UTILS_H__

namespace cppbrick {

template<typename T> const T& min(const T& a, const T& b)
{
   return a < b ? a : b;
}

template<typename T> const T& max(const T& a, const T& b)
{
   return a > b ? a : b;
}

#define DELETE_POINTER(x) do{if((x)!=NULL) delete x; x=NULL;}while(0)

#define DELETE_POINTER_ARR(x) do{if((x)!=NULL) delete []x; x=NULL;}while(0)



}

#endif

