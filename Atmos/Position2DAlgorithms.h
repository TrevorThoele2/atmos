#pragma once

#include "Position2D.h"
#include "Angle.h"

namespace Atmos
{
    Position2D::Value Length(const Position2D& position1, const Position2D& position2);
    bool IsBetween(const Position2D& position1, const Position2D& check, const Position2D& position2);
    Angle AngleOf(const Position2D& side1, const Position2D& middle, const Position2D& side2);
}