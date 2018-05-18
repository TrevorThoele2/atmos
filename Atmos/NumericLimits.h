
#pragma once

#include <limits>
#include <cstdint>

namespace Atmos
{
    template<class Type>
    struct NumericLimits
    {
        typedef Type T;
        static constexpr T min() { return std::numeric_limits<T>::min(); }
        static constexpr T lowest() { return std::numeric_limits<T>::lowest(); }
        static constexpr T max() { return std::numeric_limits<T>::max(); }
        static constexpr int digits10() { return std::numeric_limits<T>::digits10; }
        static constexpr bool is_signed = std::numeric_limits<T>::is_signed;
    };
}