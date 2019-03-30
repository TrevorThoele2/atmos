
#include "Radians.h"

#include "MathUtility.h"

namespace Atmos
{
    Radians::Radians() : AngleUnits()
    {}

    Radians::Radians(ValueT value) : AngleUnits(value)
    {}

    Radians& Radians::operator=(ValueT arg)
    {
        AngleUnits::operator=(arg);
        return *this;
    }
}