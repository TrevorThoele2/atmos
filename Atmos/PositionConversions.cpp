#include "PositionConversions.h"

#include "GridCellSize.h"

namespace Atmos
{
    Position3D ToPosition3D(const Grid::Position& position, Position3D::Value z)
    {
        return
        {
            position.x * Grid::CellSize<float>,
            position.y * Grid::CellSize<float>,
            z
        };
    }
}