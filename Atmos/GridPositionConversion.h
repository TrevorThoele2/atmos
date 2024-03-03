#pragma once

#include "GridPosition.h"
#include "Position2D.h"
#include "Position3D.h"

namespace Atmos::Grid
{
    Position ToPosition(const Position2D& position);
    Position ToPosition(const Position3D& position);
}