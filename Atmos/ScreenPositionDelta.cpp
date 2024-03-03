#include "ScreenPositionDelta.h"

namespace Atmos
{
    ScreenPositionDelta::ScreenPositionDelta(ScreenPosition from, ScreenPosition to) :
        from(from), to(to), delta{ to.x - from.x, to.y - from.y }
    {}
}