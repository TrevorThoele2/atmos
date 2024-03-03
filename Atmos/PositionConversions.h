#pragma once

#include "Position3D.h"
#include "GridPosition.h"

namespace Atmos
{
    Position3D ToPosition3D(const Grid::Position& position, Position3D::Value z);
}