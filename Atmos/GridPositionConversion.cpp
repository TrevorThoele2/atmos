#include "GridPositionConversion.h"

#include "GridCellSize.h"

namespace Atmos::Grid
{
    Position ToPosition(const Position2D& position)
    {
        return
        {
            static_cast<Position::Value>(std::floor(position.x / CellSize<Position2D::Value>)),
            static_cast<Position::Value>(std::floor(position.y / CellSize<Position2D::Value>))
        };
    }

    Position ToPosition(const Position3D& position)
    {
        return
        {
            static_cast<Position::Value>(std::floor(position.x / CellSize<Position3D::Value>)),
            static_cast<Position::Value>(std::floor(position.y / CellSize<Position3D::Value>))
        };
    }
}