#include "SpatialAlgorithms.h"

#include "GridCellSize.h"

namespace Atmos::Spatial
{
    Point2D::Value Length(const Point2D& one, const Point2D& two)
    {
        const auto distanceX = two.x - one.x;
        const auto distanceY = two.y - one.y;

        return sqrt(pow(distanceX, 2) + pow(distanceY, 2));
    }

    bool IsBetween(const Point2D& one, const Point2D& check, const Point2D& two)
    {
        const auto crossProduct =
            (check.y - one.y) *
            (two.x - one.x) -
            (check.x - one.x) *
            (two.y - one.y);

        if (std::abs(crossProduct) > std::numeric_limits<Point2D::Value>::epsilon())
            return false;

        const auto dotProduct =
            (check.x - one.x) *
            (two.x - one.x) +
            (check.y - one.y) *
            (two.y - one.y);
        if (dotProduct < 0)
            return false;

        const auto length =
            (two.x - one.x) *
            (two.x - one.x) +
            (two.y - one.y) *
            (two.y - one.y);

        return dotProduct <= length;
    }

    Angle2D AngleOf(const Point2D& side1, const Point2D& middle, const Point2D& side2)
    {
        return atan2(side2.y - middle.y, side2.x - middle.x)
            - atan2(side1.y - middle.y, side1.x - middle.x);
    }

    Point2D::Value Distance(const Point2D& starting, const Point2D& destination)
    {
        const auto distanceX = destination.x - starting.x;
        const auto distanceY = destination.y - starting.y;

        return sqrt(pow(distanceX, 2) + pow(distanceY, 2));
    }

    Point3D::Value Distance(const Point3D& starting, const Point3D& destination)
    {
        const auto distanceX = destination.x - starting.x;
        const auto distanceY = destination.y - starting.y;
        const auto distanceZ = destination.z - starting.z;

        return sqrt(pow(distanceX, 2) + pow(distanceY, 2) + pow(distanceZ, 2));
    }

    bool Contains(const AxisAlignedBox2D& box, const Point2D& point)
    {
        return point.x >= box.Left()
            && point.x <= box.Right()
            && point.y >= box.Top()
            && point.y <= box.Bottom();
    }

    bool Contains(const AxisAlignedBox2D& box, const AxisAlignedBox2D& other)
    {
        return other.Left() >= box.Left()
            && other.Right() <= box.Right()
            && other.Top() >= box.Top()
            && other.Bottom() <= box.Bottom();
    }

    bool Contains(const AxisAlignedBox3D& box, const Point3D& point)
    {
        return point.x >= box.Left()
            && point.x <= box.Right()
            && point.y >= box.Top()
            && point.y <= box.Bottom()
            && point.z >= box.FarZ()
            && point.z <= box.NearZ();
    }

    bool Contains(const AxisAlignedBox3D& box, const AxisAlignedBox3D& other)
    {
        return other.Left() >= box.Left()
            && other.Right() <= box.Right()
            && other.Top() >= box.Top()
            && other.Bottom() <= box.Bottom()
            && other.FarZ() >= box.FarZ()
            && other.NearZ() <= box.NearZ();
    }

    bool Intersects(const AxisAlignedBox2D& one, const AxisAlignedBox2D& two)
    {
        return
            (one.Left() <= two.Right() && one.Right() >= two.Left()) &&
            (one.Top() <= two.Bottom() && one.Bottom() >= two.Top());
    }

    bool Intersects(const AxisAlignedBox3D& one, const AxisAlignedBox3D& two)
    {
        return
            (one.Left() <= two.Right() && one.Right() >= two.Left()) &&
            (one.Top() <= two.Bottom() && one.Bottom() >= two.Top()) &&
            (one.FarZ() <= two.NearZ() && one.NearZ() >= two.FarZ());
    }

    Point2D operator+(const Point2D& left, const Point2D& right)
    {
        return
        {
            left.x + right.x,
            left.y + right.y
        };
    }

    Point2D operator-(const Point2D& left, const Point2D& right)
    {
        return
        {
            left.x - right.x,
            left.y - right.y
        };
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

    Point3D operator-(const Point3D& left, const Point3D& right)
    {
        return
        {
            left.x - right.x,
            left.y - right.y,
            left.z - right.z
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

    Point2D ToPoint2D(const Grid::Point& point)
    {
        return
        {
            point.x * Grid::CellSize<float>,
            point.y * Grid::CellSize<float>
        };
    }

    Point2D ToPoint2D(const Angle2D& angle, Point2D::Value distance)
    {
        return
        {
            distance * cos(angle),
            distance * sin(angle)
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
            distance * cos(angle.pitch) * cos(angle.yaw),
            distance * sin(angle.pitch)
        };
    }

    AxisAlignedBox2D ToAxisAlignedBox2D(
        float left,
        float top,
        float right,
        float bottom)
    {
        const Size2D size
        {
            right - left,
            bottom - top
        };

        const Point2D center
        {
            left + size.width / 2,
            top + size.height / 2
        };

        return AxisAlignedBox2D(center, size);
    }

    AxisAlignedBox3D ToAxisAlignedBox3D(
        float left,
        float top,
        float farZ,
        float right,
        float bottom,
        float nearZ)
    {
        const Size3D size
        {
            right - left,
            bottom - top,
            nearZ - farZ
        };

        const Point3D center
        {
            left + size.width / 2,
            top + size.height / 2,
            farZ + size.depth / 2
        };

        return AxisAlignedBox3D(center, size);
    }

    namespace Grid
    {
        Point::Value Length(const Point& position1, const Point& position2)
        {
            return position2.x - position1.x + position2.y - position1.y;
        }

        Point ToPoint(const Point2D& position)
        {
            return
            {
                static_cast<Point::Value>(std::floor(position.x / CellSize<Point2D::Value>)),
                static_cast<Point::Value>(std::floor(position.y / CellSize<Point2D::Value>))
            };
        }

        Point ToPoint(const Point3D& position)
        {
            return
            {
                static_cast<Point::Value>(std::floor(position.x / CellSize<Point3D::Value>)),
                static_cast<Point::Value>(std::floor(position.y / CellSize<Point3D::Value>))
            };
        }
    }
}