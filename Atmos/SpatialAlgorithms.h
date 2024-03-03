#pragma once

#include "Point2D.h"
#include "Point3D.h"
#include "ScreenPoint.h"
#include "GridPoint.h"
#include "Angle2D.h"
#include "Angle3D.h"
#include "AxisAlignedBox2D.h"
#include "AxisAlignedBox3D.h"
#include "GridAxisAlignedBox.h"

namespace Atmos::Spatial
{
    [[nodiscard]] Point2D::Value Distance(Point2D starting, Point2D destination);
    [[nodiscard]] Point3D::Value Distance(Point3D starting, Point3D destination);

    [[nodiscard]] bool IsBetween(Point2D one, Point2D check, Point2D two);
    [[nodiscard]] Angle2D AngleOf(Point2D side1, Point2D middle, Point2D side2);

    [[nodiscard]] bool Contains(AxisAlignedBox2D box, Point2D point);
    [[nodiscard]] bool Contains(AxisAlignedBox2D box, AxisAlignedBox2D other);
    [[nodiscard]] bool Contains(AxisAlignedBox3D box, Point3D point);
    [[nodiscard]] bool Contains(AxisAlignedBox3D box, AxisAlignedBox3D other);

    [[nodiscard]] bool Intersects(AxisAlignedBox2D one, AxisAlignedBox2D two);
    [[nodiscard]] bool Intersects(AxisAlignedBox3D one, AxisAlignedBox3D two);

    [[nodiscard]] Point2D operator+(Point2D left, Point2D right);
    [[nodiscard]] Point2D operator-(Point2D left, Point2D right);
    [[nodiscard]] Point3D operator+(Point3D left, Point3D right);
    [[nodiscard]] Point3D operator-(Point3D left, Point3D right);
    [[nodiscard]] ScreenPoint operator+(ScreenPoint left, ScreenPoint right);
    [[nodiscard]] ScreenPoint operator-(ScreenPoint left, ScreenPoint right);

    [[nodiscard]] Point2D ToPoint2D(Point3D point);
    [[nodiscard]] Point2D ToPoint2D(Grid::Point point);
    [[nodiscard]] Point2D ToPoint2D(Angle2D angle, Point2D::Value distance);
    [[nodiscard]] Point3D ToPoint3D(Point2D point, Point3D::Value z);
    [[nodiscard]] Point3D ToPoint3D(Grid::Point point, Point3D::Value z);
    [[nodiscard]] Point3D ToPoint3D(Angle3D angle, Point3D::Value distance);

    [[nodiscard]] Angle2D ToDegrees(Angle2D angle);

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
        [[nodiscard]] Point::Value Distance(Point starting, Point destination);

        [[nodiscard]] bool Contains(AxisAlignedBox box, Point point);
        [[nodiscard]] bool Contains(AxisAlignedBox box, AxisAlignedBox other);
        [[nodiscard]] bool Intersects(AxisAlignedBox one, AxisAlignedBox two);

        [[nodiscard]] Point ToPoint(Point2D position);
        [[nodiscard]] Point ToPoint(Point3D position);
    }
}