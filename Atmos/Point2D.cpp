#include "Point2D.h"

namespace Atmos::Spatial
{
    bool Point2D::operator==(const Point2D& arg) const
    {
        return x == arg.x && y == arg.y;
    }

    bool Point2D::operator!=(const Point2D& arg) const
    {
        return !(*this == arg);
    }
}