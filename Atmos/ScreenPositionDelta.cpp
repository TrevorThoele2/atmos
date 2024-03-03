#include "ScreenPositionDelta.h"

namespace Atmos
{
    ScreenPositionDelta::ScreenPositionDelta(ScreenPosition from, ScreenPosition to) :
        from(from), to(to)
    {}

    ScreenPosition ScreenPositionDelta::Delta() const
    {
        return { to.x - from.x, to.y - from.y };
    }
}