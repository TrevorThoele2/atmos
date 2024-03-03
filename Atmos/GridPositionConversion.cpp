#include "GridPositionConversion.h"

#include "GridCellSize.h"

namespace Atmos::Grid
{
    Position ToPosition(const Position2D& position)
    {
        return
        {
            static_cast<Position::Value>(position.x) / CellSize<Position::Value>,
            static_cast<Position::Value>(position.y) / CellSize<Position::Value>
        };
    }

    Position ToPosition(const Position3D& position)
    {
        return
        {
            static_cast<Position::Value>(position.x) / CellSize<Position::Value>,
            static_cast<Position::Value>(position.y) / CellSize<Position::Value>
        };
    }
}