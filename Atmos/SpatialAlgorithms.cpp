#include "SpatialAlgorithms.h"

#include "GridCellSize.h"

namespace Atmos::Spatial
{
    Point2D::Value Length(const Point2D& point1, const Point2D& point2)
    {
        const auto distanceX = point2.x - point1.x;
        const auto distanceY = point2.y - point1.y;

        return sqrt(pow(distanceX, 2) + pow(distanceY, 2));
    }

    bool IsBetween(const Point2D& point1, const Point2D& check, const Point2D& point2)
    {
        const auto crossProduct =
            (check.y - point1.y) *
            (point2.x - point1.x) -
            (check.x - point1.x) *
            (point2.y - point1.y);

        if (std::abs(crossProduct) > std::numeric_limits<Point2D::Value>::epsilon())
            return false;

        const auto dotProduct =
            (check.x - point1.x) *
            (point2.x - point1.x) +
            (check.y - point1.y) *
            (point2.y - point1.y);
        if (dotProduct < 0)
            return false;

        const auto length =
            (point2.x - point1.x) *
            (point2.x - point1.x) +
            (point2.y - point1.y) *
            (point2.y - point1.y);

        return dotProduct <= length;
    }

    Angle2D AngleOf(const Point2D& side1, const Point2D& middle, const Point2D& side2)
    {
        return atan2(side2.y - middle.y, side2.x - middle.x)
            - atan2(side1.y - middle.y, side1.x - middle.x);
    }

    Point3D operator+(const Point3D& left, const Point3D& right)
    {
        return
        {
            left.x + right.x,
            left.y + right.y,
            left.z + right.z
        };
    }

    Point2D ToPoint2D(const Point3D& point)
    {
        return
        {
            point.x,
            point.y
        };
    }

    Point3D ToPoint3D(const Grid::Point& point, Point3D::Value z)
    {
        return
        {
            point.x * Grid::CellSize<float>,
            point.y * Grid::CellSize<float>,
            z
        };
    }

    Point3D ToPoint3D(const Angle3D& angle, Point3D::Value distance)
    {
        return
        {
            distance * cos(angle.pitch) * sin(angle.yaw),
            distance* cos(angle.pitch) * cos(angle.yaw),
            distance* sin(angle.pitch)
        };
    }
}