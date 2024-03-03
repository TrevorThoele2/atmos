#pragma once

#include <type_traits>

#include "AngleUnits.h"

namespace Atmos
{
    class Radians : public AngleUnits<Radians>
    {
    public:
        Radians();
        Radians(ValueT value);
        Radians& operator=(ValueT arg);
    };
}

namespace std
{
    template<>
    struct is_arithmetic<::Atmos::Radians> : public std::true_type
    {};
}