#pragma once

#include "Point2D.h"
#include "Point3D.h"
#include "GridPoint.h"
#include "Angle2D.h"
#include "Angle3D.h"
#include "AxisAlignedBox2D.h"
#include "AxisAlignedBox3D.h"

namespace Atmos::Spatial
{
    [[nodiscard]] Point2D::Value Length(const Point2D& one, const Point2D& two);
    [[nodiscard]] bool IsBetween(const Point2D& one, const Point2D& check, const Point2D& two);
    [[nodiscard]] Angle2D AngleOf(const Point2D& side1, const Point2D& middle, const Point2D& side2);

    [[nodiscard]] Point2D::Value Distance(const Point2D& starting, const Point2D& destination);
    [[nodiscard]] Point3D::Value Distance(const Point3D& starting, const Point3D& destination);

    [[nodiscard]] bool Contains(const AxisAlignedBox2D& box, const Point2D& point);
    [[nodiscard]] bool Contains(const AxisAlignedBox2D& box, const AxisAlignedBox2D& other);
    [[nodiscard]] bool Contains(const AxisAlignedBox3D& box, const Point3D& point);
    [[nodiscard]] bool Contains(const AxisAlignedBox3D& box, const AxisAlignedBox3D& other);

    [[nodiscard]] bool Intersects(const AxisAlignedBox2D& one, const AxisAlignedBox2D& two);
    [[nodiscard]] bool Intersects(const AxisAlignedBox3D& one, const AxisAlignedBox3D& two);

    [[nodiscard]] Point2D operator+(const Point2D& left, const Point2D& right);
    [[nodiscard]] Point2D operator-(const Point2D& left, const Point2D& right);
    [[nodiscard]] Point3D operator+(const Point3D& left, const Point3D& right);
    [[nodiscard]] Point3D operator-(const Point3D& left, const Point3D& right);

    [[nodiscard]] Point2D ToPoint2D(const Point3D& point);
    [[nodiscard]] Point2D ToPoint2D(const Grid::Point& point);
    [[nodiscard]] Point2D ToPoint2D(const Angle2D& angle, Point2D::Value distance);
    [[nodiscard]] Point3D ToPoint3D(const Grid::Point& point, Point3D::Value z);
    [[nodiscard]] Point3D ToPoint3D(const Angle3D& angle, Point3D::Value distance);

    [[nodiscard]] AxisAlignedBox2D ToAxisAlignedBox2D(
        float left,
        float top,
        float right,
        float bottom);
    [[nodiscard]] AxisAlignedBox3D ToAxisAlignedBox3D(
        float left,
        float top,
        float farZ,
        float right,
        float bottom,
        float nearZ);

    namespace Grid
    {
        [[nodiscard]] Point::Value Length(const Point& position1, const Point& position2);

        [[nodiscard]] Point ToPoint(const Point2D& position);
        [[nodiscard]] Point ToPoint(const Point3D& position);
    }
}