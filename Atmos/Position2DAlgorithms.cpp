#include "Position2DAlgorithms.h"

namespace Atmos
{
    Position2D::Value Length(const Position2D& position1, const Position2D& position2)
    {
        const auto distanceX = position2.x - position1.x;
        const auto distanceY = position2.y - position1.y;

        return sqrt(pow(distanceX, 2) + pow(distanceY, 2));
    }

    bool IsBetween(const Position2D& position1, const Position2D& check, const Position2D& position2)
    {
        const auto crossProduct =
            (check.y - position1.y) *
            (position2.x - position1.x) -
            (check.x - position1.x) *
            (position2.y - position1.y);

        if (std::abs(crossProduct) > std::numeric_limits<Position2D::Value>::epsilon())
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

    Angle AngleOf(const Position2D& side1, const Position2D& middle, const Position2D& side2)
    {
        return atan2(side2.y - middle.y, side2.x - middle.x)
            - atan2(side1.y - middle.y, side1.x - middle.x);
    }
}