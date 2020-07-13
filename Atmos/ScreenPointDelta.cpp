#include "ScreenPointDelta.h"

namespace Atmos::Spatial
{
    ScreenPointDelta::ScreenPointDelta(ScreenPoint from, ScreenPoint to) :
        from(from), to(to)
    {}

    ScreenPoint ScreenPointDelta::Delta() const
    {
        return { to.x - from.x, to.y - from.y };
    }
}