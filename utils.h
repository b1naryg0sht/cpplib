#ifndef __YOGURT_UTILS_H__
#define __YOGURT_UTILS_H__

namespace Yogurt {
    template<typename T> const T& min(const T& a, const T& b)
    {
       return a < b ? a : b;
    }

    template<typename T> const T& max(const T& a, const T& b)
    {
       return a > b ? a : b;
    }
}

#endif //__YOGURT_UTILS_H__

