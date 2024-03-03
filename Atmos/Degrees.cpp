
#include "Degrees.h"

namespace Atmos
{
    Degrees::Degrees() : AngleUnits()
    {}

    Degrees::Degrees(ValueT value) : AngleUnits(value)
    {}

    Degrees& Degrees::operator=(ValueT arg)
    {
        AngleUnits::operator=(arg);
        return *this;
    }
}