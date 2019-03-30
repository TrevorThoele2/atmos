#pragma once

#include <type_traits>

#include "AngleUnits.h"

namespace Atmos
{
    class Degrees : public AngleUnits<Degrees>
    {
    public:
        Degrees();
        Degrees(ValueT value);
        Degrees& operator=(ValueT arg);
    };
}

namespace std
{
    template<>
    struct is_arithmetic<::Atmos::Degrees> : public std::true_type
    {};
}