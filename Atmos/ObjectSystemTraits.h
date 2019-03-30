#pragma once

#include <type_traits>

#include "ObjectSystemPriority.h"

namespace Atmos
{
    template<class T>
    struct ObjectSystemTraits;

    template <class T, class = int>
    struct HasPriority : std::false_type
    {};

    template <class T>
    struct HasPriority<T, decltype((void)ObjectSystemTraits<T>::priority, 0)> : std::true_type
    {};

    template<class T, typename std::enable_if<HasPriority<T>::value, int>::type = 0>
    ObjectSystemPriority PriorityFor()
    {
        return ObjectSystemTraits<T>::priority;
    }

    template<class T, typename std::enable_if<!HasPriority<T>::value, int>::type = 0>
    ObjectSystemPriority PriorityFor()
    {
        return 0;
    }
}