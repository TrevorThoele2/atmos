#include "Property.h"

namespace Atmos
{
    bool Property::operator==(const Property& arg) const
    {
        return name == arg.name && value == arg.value;
    }

    bool Property::operator!=(const Property& arg) const
    {
        return !(*this == arg);
    }
}