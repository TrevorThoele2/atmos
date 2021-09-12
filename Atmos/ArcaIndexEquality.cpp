#include "ArcaIndexEquality.h"

#include <Arca/RelicIndex.h>
#include <Arca/OpenRelic.h>

namespace Atmos
{
    bool IndexEqualsValue(const Arca::Index<Arca::OpenRelic>& left, const Arca::Index<Arca::OpenRelic>& right)
    {
        return !left || !right ? !left && !right : true;
    }
}