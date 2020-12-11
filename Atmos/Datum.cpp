#include "Datum.h"

namespace Atmos
{
    bool Datum::operator==(const Datum& arg) const
    {
        return name == arg.name && value == arg.value;
    }

    bool Datum::operator!=(const Datum& arg) const
    {
        return !(*this == arg);
    }
}