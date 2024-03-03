
#include "AngleConversion.h"

#include "MathUtility.h"

namespace Atmos
{
    AngleConverter<Radians>::ToT AngleConverter<Radians>::To(FromT from)
    {
        return from;
    }

    AngleConverter<Radians>::FromT AngleConverter<Radians>::From(ToT from)
    {
        return from;
    }

    AngleConverter<Degrees>::ToT AngleConverter<Degrees>::To(FromT from)
    {
        return Degrees((static_cast<ValueT>(from) * 180.0f) / PI<float>);
    }

    AngleConverter<Degrees>::FromT AngleConverter<Degrees>::From(ToT from)
    {
        return Radians((static_cast<ValueT>(from) * PI<float>) / 180.0f);
    }
}