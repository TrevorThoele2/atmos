#include "GridPointAlgorithms.h"

namespace Atmos::Spatial::Grid
{
    Point::Value Length(const Point& position1, const Point& position2)
    {
        return position2.x - position1.x + position2.y - position1.y;
    }
}