#pragma once

#include <type_traits>

namespace Atmos
{
    template<typename T, typename = void>
    struct HasEqualityComparison : std::false_type
    {};

    template<typename T>
    struct HasEqualityComparison<T,typename std::enable_if<true, decltype(std::declval<T&>() == std::declval<T&>(), (void)0)>::type> : std::true_type
    {};
}