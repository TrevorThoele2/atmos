#include "GridPoint.h"

namespace Atmos::Spatial::Grid
{
    bool Point::operator==(const Point& arg) const
    {
        return x == arg.x && y == arg.y;
    }

    bool Point::operator!=(const Point& arg) const
    {
        return !(*this == arg);
    }
}