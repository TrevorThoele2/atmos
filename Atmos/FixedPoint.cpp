
#include "FixedPoint.h"

namespace Atmos
{
    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(RadixPoint)
    {
        scribe(value);
    }

    RadixPoint& RadixPoint::operator=(ValueT arg)
    {
        value = arg;
        return *this;
    }

    bool RadixPoint::operator==(const RadixPoint &arg) const
    {
        return value == arg.value;
    }

    bool RadixPoint::operator!=(const RadixPoint &arg) const
    {
        return !(*this == arg);
    }

    RadixPoint::operator ValueT() const
    {
        return value;
    }

    RadixPoint::ValueT RadixPoint::Get() const
    {
        return value;
    }
}