#pragma once

#include "Point2D.h"
#include "Point3D.h"
#include "GridPoint.h"

namespace Atmos::Spatial
{
    Point2D ToPoint2D(const Point3D& position);
    Point3D ToPoint3D(const Grid::Point& position, Point3D::Value z);
}