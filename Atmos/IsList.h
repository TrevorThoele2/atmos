#pragma once

#include <type_traits>
#include <vector>
#include <list>

namespace Atmos
{
    template<typename T>
    struct IsList : std::false_type
    {};

    template<typename T>
    struct IsList<std::vector<T>> : std::true_type
    {};

    template<typename T>
    struct IsList<std::list<T>> : std::true_type
    {};
}