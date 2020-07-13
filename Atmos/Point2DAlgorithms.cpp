#include "Point2DAlgorithms.h"

namespace Atmos::Spatial
{
    Point2D::Value Length(const Point2D& position1, const Point2D& position2)
    {
        const auto distanceX = position2.x - position1.x;
        const auto distanceY = position2.y - position1.y;

        return sqrt(pow(distanceX, 2) + pow(distanceY, 2));
    }

    bool IsBetween(const Point2D& position1, const Point2D& check, const Point2D& position2)
    {
        const auto crossProduct =
            (check.y - position1.y) *
            (position2.x - position1.x) -
            (check.x - position1.x) *
            (position2.y - position1.y);

        if (std::abs(crossProduct) > std::numeric_limits<Point2D::Value>::epsilon())
            return false;

        const auto dotProduct =
            (check.x - position1.x) *
            (position2.x - position1.x) +
            (check.y - position1.y) *
            (position2.y - position1.y);
        if (dotProduct < 0)
            return false;

        const auto length =
            (position2.x - position1.x) *
            (position2.x - position1.x) +
            (position2.y - position1.y) *
            (position2.y - position1.y);

        return dotProduct <= length;
    }

    Angle AngleOf(const Point2D& side1, const Point2D& middle, const Point2D& side2)
    {
        return atan2(side2.y - middle.y, side2.x - middle.x)
            - atan2(side1.y - middle.y, side1.x - middle.x);
    }
}