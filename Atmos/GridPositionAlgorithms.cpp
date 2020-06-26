#include "GridPositionAlgorithms.h"

namespace Atmos::Grid
{
    Position::Value Length(const Position& position1, const Position& position2)
    {
        return position2.x - position1.x + position2.y - position1.y;
    }
}