#include "ScreenPoint.h"

namespace Atmos::Spatial
{
    ScreenPoint::ScreenPoint() : x(0), y(0)
    {}

    ScreenPoint::ScreenPoint(Value x, Value y) : x(x), y(y)
    {}

    bool ScreenPoint::operator==(const ScreenPoint& arg) const
    {
        return x == arg.x && y == arg.y;
    }

    bool ScreenPoint::operator!=(const ScreenPoint& arg) const
    {
        return !(*this == arg);
    }
}