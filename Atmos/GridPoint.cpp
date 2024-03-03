#include "GridPoint.h"

#include <Inscription/Scribe.h>

namespace Atmos::Spatial::Grid
{
    Point::Point(Value x, Value y) : x(x), y(y)
    {}

    bool Point::operator==(const Point& arg) const
    {
        return x == arg.x && y == arg.y;
    }

    bool Point::operator!=(const Point& arg) const
    {
        return !(*this == arg);
    }
}