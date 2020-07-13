#pragma once

#include "GridPoint.h"
#include "Point2D.h"
#include "Point3D.h"

namespace Atmos::Spatial::Grid
{
    Point ToPoint(const Point2D& position);
    Point ToPoint(const Point3D& position);
}