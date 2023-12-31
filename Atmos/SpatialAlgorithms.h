#pragma once

#include "Point2D.h"
#include "Point3D.h"
#include "Scalers2D.h"
#include "Scalers3D.h"
#include "GridPoint.h"
#include "Angle2D.h"
#include "Angle3D.h"
#include "Line2D.h"
#include "AxisAlignedBox2D.h"
#include "AxisAlignedBox3D.h"
#include "GridAxisAlignedBox.h"

namespace Atmos::Spatial
{
    enum class Side
    {
        Left,
        On,
        Right
    };

    [[nodiscard]] Point2D::Value Distance(Point2D starting, Point2D destination);
    [[nodiscard]] Point3D::Value Distance(Point3D starting, Point3D destination);

    [[nodiscard]] bool IsBetween(Point2D one, Point2D check, Point2D two);
    [[nodiscard]] Angle2D AngleOf(Point2D side1, Point2D middle, Point2D side2);

    [[nodiscard]] bool Contains(AxisAlignedBox2D box, Point2D point);
    [[nodiscard]] bool Contains(AxisAlignedBox2D box, AxisAlignedBox2D other);
    [[nodiscard]] bool Contains(AxisAlignedBox3D box, Point3D point);
    [[nodiscard]] bool Contains(AxisAlignedBox3D box, AxisAlignedBox3D other);
    [[nodiscard]] bool Contains(Line2D line, Point2D point);

    [[nodiscard]] bool Intersects(AxisAlignedBox2D one, AxisAlignedBox2D two);
    [[nodiscard]] bool Intersects(AxisAlignedBox3D one, AxisAlignedBox3D two);

    [[nodiscard]] std::optional<Point2D> Intersection(Line2D one, Line2D two);

    [[nodiscard]] AxisAlignedBox2D Envelope(std::vector<AxisAlignedBox2D> boxes);
    [[nodiscard]] AxisAlignedBox3D Envelope(std::vector<AxisAlignedBox3D> boxes);

    [[nodiscard]] AxisAlignedBox2D Clamp(AxisAlignedBox2D box, AxisAlignedBox2D to);
    [[nodiscard]] AxisAlignedBox3D Clamp(AxisAlignedBox3D box, AxisAlignedBox3D to);

    [[nodiscard]] Size2D ScaleBy(Size2D size, Scalers2D scalers);
    [[nodiscard]] Size3D ScaleBy(Size3D size, Scalers3D scalers);

    [[nodiscard]] AxisAlignedBox2D ScaleOf(AxisAlignedBox2D box, AxisAlignedBox2D against);
    [[nodiscard]] AxisAlignedBox3D ScaleOf(AxisAlignedBox3D box, AxisAlignedBox3D against);

    [[nodiscard]] Side SideOf(Line2D line, Point2D point);

    [[nodiscard]] AxisAlignedBox2D Clip(AxisAlignedBox2D box, AxisAlignedBox2D clip);
    [[nodiscard]] std::vector<Point2D> Clip(std::vector<Point2D> points, std::vector<Point2D> clip);

    [[nodiscard]] AxisAlignedBox2D Cell(int column, int row, Size2D cellSize);
    [[nodiscard]] AxisAlignedBox3D Cell(int column, int row, int depth, Size3D cellSize);

    [[nodiscard]] Point2D Rotate(Point2D point, Angle2D rotation, Point2D rotationCenter);

    [[nodiscard]] Point2D operator+(Point2D left, Point2D right);
    [[nodiscard]] Point3D operator+(Point3D left, Point3D right);
    [[nodiscard]] AxisAlignedBox2D operator+(AxisAlignedBox2D left, Point2D right);
    [[nodiscard]] AxisAlignedBox3D operator+(AxisAlignedBox3D left, Point3D right);
    [[nodiscard]] Point2D operator-(Point2D left, Point2D right);
    [[nodiscard]] Point3D operator-(Point3D left, Point3D right);
    [[nodiscard]] AxisAlignedBox2D operator-(AxisAlignedBox2D left, Point2D right);
    [[nodiscard]] AxisAlignedBox3D operator-(AxisAlignedBox3D left, Point3D right);

    [[nodiscard]] Point2D ToPoint2D(Point3D point);
    [[nodiscard]] Point2D ToPoint2D(Grid::Point point);
    [[nodiscard]] Point2D ToPoint2D(Angle2D angle, Point2D::Value distance);
    [[nodiscard]] Point3D ToPoint3D(Point2D point, Point3D::Value z);
    [[nodiscard]] Point3D ToPoint3D(Grid::Point point, Point3D::Value z);
    [[nodiscard]] Point3D ToPoint3D(Angle3D angle, Point3D::Value distance);

    [[nodiscard]] std::vector<Point2D> ToPoints(
        AxisAlignedBox2D box,
        Spatial::Angle2D rotation,
        Spatial::Point2D rotationCenter);

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

        [[nodiscard]] Point ToPoint(Point2D point);
        [[nodiscard]] Point ToPoint(Point3D point);
    }
}