#pragma once

#include <type_traits>

namespace Atmos
{
    template<class T>
    typename std::underlying_type<T>::type ConvertToUnderlyingType(T t)
    {
        return static_cast<std::underlying_type<T>::type>(t);
    }
}