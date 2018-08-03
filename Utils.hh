#ifndef _YOGURT_UTILS_HH
#define _YOGURT_UTILS_HH

namespace Yogurt
{
    template<typename T> const T& min(const T& a, const T& b)
    {
       return a < b ? a : b;
    }

    template<typename T> const T& max(const T& a, const T& b)
    {
       return a > b ? a : b;
    }
}

#endif //_YOGURT_UTILS_HH

