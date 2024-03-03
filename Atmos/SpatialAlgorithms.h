#pragma once

#include "Point2D.h"
#include "Point3D.h"
#include "GridPoint.h"
#include "Angle2D.h"
#include "Angle3D.h"

namespace Atmos::Spatial
{
    Point2D::Value Length(const Point2D& point1, const Point2D& point2);
    bool IsBetween(const Point2D& point1, const Point2D& check, const Point2D& point2);
    Angle2D AngleOf(const Point2D& side1, const Point2D& middle, const Point2D& side2);

    Point3D operator+(const Point3D& left, const Point3D& right);

    Point2D ToPoint2D(const Point3D& point);
    Point3D ToPoint3D(const Grid::Point& point, Point3D::Value z);
    Point3D ToPoint3D(const Angle3D& angle, Point3D::Value distance);
}