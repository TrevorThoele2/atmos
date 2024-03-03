#pragma once

#include "Point2D.h"
#include "Angle.h"

namespace Atmos::Spatial
{
    Point2D::Value Length(const Point2D& position1, const Point2D& position2);
    bool IsBetween(const Point2D& position1, const Point2D& check, const Point2D& position2);
    Angle AngleOf(const Point2D& side1, const Point2D& middle, const Point2D& side2);
}