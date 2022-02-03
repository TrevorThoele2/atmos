#include "SpatialAlgorithms.h"

#include "MathUtility.h"
#include "GridCellSize.h"

namespace Atmos::Spatial
{
    template<class T>
    [[nodiscard]] T Determinant(T a, T b, T c, T d)
    {
        return a * d - b * c;
    }

    Point2D::Value Distance(Point2D starting, Point2D destination)
    {
        const auto distanceX = destination.x - starting.x;
        const auto distanceY = destination.y - starting.y;

        return static_cast<Point2D::Value>(sqrt(pow(distanceX, 2) + pow(distanceY, 2)));
    }

    Point3D::Value Distance(Point3D starting, Point3D destination)
    {
        const auto distanceX = destination.x - starting.x;
        const auto distanceY = destination.y - starting.y;
        const auto distanceZ = destination.z - starting.z;

        return static_cast<Point3D::Value>(sqrt(pow(distanceX, 2) + pow(distanceY, 2) + pow(distanceZ, 2)));
    }

    bool IsBetween(Point2D one, Point2D check, Point2D two)
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

    Angle2D AngleOf(Point2D side1, Point2D middle, Point2D side2)
    {
        return atan2(side2.y - middle.y, side2.x - middle.x)
            - atan2(side1.y - middle.y, side1.x - middle.x);
    }

    bool Contains(AxisAlignedBox2D box, Point2D point)
    {
        return point.x >= box.Left()
            && point.x <= box.Right()
            && point.y >= box.Top()
            && point.y <= box.Bottom();
    }

    bool Contains(AxisAlignedBox2D box, AxisAlignedBox2D other)
    {
        return other.Left() >= box.Left()
            && other.Right() <= box.Right()
            && other.Top() >= box.Top()
            && other.Bottom() <= box.Bottom();
    }

    bool Contains(AxisAlignedBox3D box, Point3D point)
    {
        return point.x >= box.Left()
            && point.x <= box.Right()
            && point.y >= box.Top()
            && point.y <= box.Bottom()
            && point.z >= box.FarZ()
            && point.z <= box.NearZ();
    }

    bool Contains(AxisAlignedBox3D box, AxisAlignedBox3D other)
    {
        return other.Left() >= box.Left()
            && other.Right() <= box.Right()
            && other.Top() >= box.Top()
            && other.Bottom() <= box.Bottom()
            && other.FarZ() >= box.FarZ()
            && other.NearZ() <= box.NearZ();
    }

    bool Contains(Line2D line, Point2D point)
    {
        const auto a = line.from;
        const auto b = point;
        const auto c = line.to;

        const auto cross = (c.y - a.y) * (b.x - a.x) - (c.x - a.x) * (b.y - a.y);
        if (std::abs(cross) > std::numeric_limits<float>::epsilon())
            return false;

        const auto dot = (c.x - a.x) * (b.x - a.x) + (c.y - a.y) * (b.y - a.y);
        if (dot < 0)
            return false;

        const auto squaredLength = Power(b.x - a.x, 2) + Power(b.y - a.y, 2);
        return dot > squaredLength;
    }

    bool Intersects(AxisAlignedBox2D one, AxisAlignedBox2D two)
    {
        return
            (one.Left() <= two.Right() && one.Right() >= two.Left()) &&
            (one.Top() <= two.Bottom() && one.Bottom() >= two.Top());
    }

    bool Intersects(AxisAlignedBox3D one, AxisAlignedBox3D two)
    {
        return
            (one.Left() <= two.Right() && one.Right() >= two.Left()) &&
            (one.Top() <= two.Bottom() && one.Bottom() >= two.Top()) &&
            (one.FarZ() <= two.NearZ() && one.NearZ() >= two.FarZ());
    }

    std::optional<Point2D> Intersection(Line2D one, Line2D two)
    {
        const auto oneX = one.from.x - one.to.x;
        const auto oneY = one.from.y - one.to.y;
        const auto twoX = two.from.x - two.to.x;
        const auto twoY = two.from.y - two.to.y;

        const auto denominator = Determinant(oneX, oneY, twoX, twoY);
        if (denominator == 0)
            return {};

        const auto oneDeterminant = Determinant(one.from.x, one.from.y, one.to.x, one.to.y);
        const auto twoDeterminant = Determinant(two.from.x, two.from.y, two.to.x, two.to.y);

        const auto intersection = Point2D
        {
            Determinant(oneDeterminant, oneX, twoDeterminant, twoX) / denominator,
            Determinant(oneDeterminant, oneY, twoDeterminant, twoY) / denominator
        };

        return intersection;
    }

    AxisAlignedBox2D Envelope(std::vector<AxisAlignedBox2D> boxes)
    {
        AxisAlignedBox2D::Coordinate left = 0;
        AxisAlignedBox2D::Coordinate top = 0;
        AxisAlignedBox2D::Coordinate right = 0;
        AxisAlignedBox2D::Coordinate bottom = 0;

        for(auto& box : boxes)
        {
            left = std::min(left, box.Left());
            top = std::min(top, box.Top());
            right = std::max(right, box.Right());
            bottom = std::max(bottom, box.Bottom());
        }

        return ToAxisAlignedBox2D(left, top, right, bottom);
    }

    AxisAlignedBox3D Envelope(std::vector<AxisAlignedBox3D> boxes)
    {
        AxisAlignedBox3D::Coordinate left = 0;
        AxisAlignedBox3D::Coordinate top = 0;
        AxisAlignedBox3D::Coordinate farZ = 0;
        AxisAlignedBox3D::Coordinate right = 0;
        AxisAlignedBox3D::Coordinate bottom = 0;
        AxisAlignedBox3D::Coordinate nearZ = 0;

        for (auto& box : boxes)
        {
            left = std::min(left, box.Left());
            top = std::min(top, box.Top());
            farZ = std::min(farZ, box.FarZ());
            right = std::max(right, box.Right());
            bottom = std::max(bottom, box.Bottom());
            nearZ = std::max(nearZ, box.NearZ());
        }

        return ToAxisAlignedBox3D(left, top, farZ, right, bottom, nearZ);
    }

    AxisAlignedBox2D Clamp(AxisAlignedBox2D box, AxisAlignedBox2D to)
    {
        return ToAxisAlignedBox2D(
            std::clamp(box.Left(), to.Left(), to.Right()),
            std::clamp(box.Top(), to.Top(), to.Bottom()),
            std::clamp(box.Right(), to.Left(), to.Right()),
            std::clamp(box.Bottom(), to.Top(), to.Bottom()));
    }

    AxisAlignedBox3D Clamp(AxisAlignedBox3D box, AxisAlignedBox3D to)
    {
        return ToAxisAlignedBox3D(
            std::clamp(box.Left(), to.Left(), to.Right()),
            std::clamp(box.Top(), to.Top(), to.Bottom()),
            std::clamp(box.FarZ(), to.FarZ(), to.NearZ()),
            std::clamp(box.Right(), to.Left(), to.Right()),
            std::clamp(box.Bottom(), to.Top(), to.Bottom()),
            std::clamp(box.NearZ(), to.FarZ(), to.NearZ()));
    }

    Size2D ScaleBy(Size2D size, Scalers2D scalers)
    {
        return { size.width * scalers.x, size.height * scalers.y };
    }

    Size3D ScaleBy(Size3D size, Scalers3D scalers)
    {
        return { size.width * scalers.x, size.height * scalers.y, size.depth * scalers.z };
    }

    AxisAlignedBox2D ScaleOf(AxisAlignedBox2D box, AxisAlignedBox2D against)
    {
        return ToAxisAlignedBox2D(
            (box.Left() - against.Left()) / against.size.width,
            (box.Top() - against.Top()) / against.size.height,
            (box.Right() - against.Left()) / against.size.width,
            (box.Bottom() - against.Top()) / against.size.height);
    }

    AxisAlignedBox3D ScaleOf(AxisAlignedBox3D box, AxisAlignedBox3D against)
    {
        return ToAxisAlignedBox3D(
            (box.Left() - against.Left()) / against.size.width,
            (box.Top() - against.Top()) / against.size.height,
            (box.FarZ() - against.FarZ()) / against.size.depth,
            (box.Right() - against.Left()) / against.size.width,
            (box.Bottom() - against.Top()) / against.size.height,
            (box.NearZ() - against.FarZ()) / against.size.depth);
    }

    Side SideOf(Line2D line, Point2D point)
    {
        const auto [from, to] = line;

        const auto value = (to.x - from.x) * (point.y - from.y) - (to.y - from.y) * (point.x - from.x);
        if (value == 0)
            return Side::On;
        else if (value > 0)
            return Side::Left;
        else
            return Side::Right;
    }

    std::vector<Point2D> Clip(std::vector<Point2D> points, std::vector<Point2D> clip)
    {
        // Sutherland-Hodgman algorithm
        auto outputList = points;

        for (int i = 0; static_cast<size_t>(i) < clip.size(); ++i)
        {
            const auto clipEdge = Line2D{ clip[(i - 1) % clip.size()], clip[i] };
            
            auto inputList = outputList;
            outputList.clear();

            for (int j = 0; static_cast<size_t>(j) < inputList.size(); ++j)
            {
                const auto currentPoint = inputList[j];
                const auto previousPoint = inputList[(j - 1) % inputList.size()];

                const auto intersectingPoint = Intersection(Line2D{ previousPoint, currentPoint }, clipEdge);
                const auto currentSide = SideOf(clipEdge, currentPoint);
                const auto previousSide = SideOf(clipEdge, previousPoint);
                if (currentSide == Side::Right)
                {
                    if (previousSide == Side::Left)
                        outputList.push_back(*intersectingPoint);
                    outputList.push_back(currentPoint);
                }
                else if (previousSide == Side::Right)
                    outputList.push_back(*intersectingPoint);
            }
        }

        return outputList;
    }

    AxisAlignedBox2D Cell(int column, int row, Size2D cellSize)
    {
        return Spatial::ToAxisAlignedBox2D
        (
            column * cellSize.width,
            row * cellSize.height,
            column * cellSize.width + cellSize.width,
            row * cellSize.height + cellSize.height
        );
    }

    AxisAlignedBox3D Cell(int column, int row, int depth, Size3D cellSize)
    {
        return Spatial::ToAxisAlignedBox3D
        (
            column * cellSize.width,
            row * cellSize.height,
            depth * cellSize.depth,
            column * cellSize.width + cellSize.width,
            row * cellSize.height + cellSize.height,
            depth * cellSize.depth + cellSize.depth
        );
    }

    Point2D operator+(Point2D left, Point2D right)
    {
        return
        {
            left.x + right.x,
            left.y + right.y
        };
    }

    Point3D operator+(Point3D left, Point3D right)
    {
        return
        {
            left.x + right.x,
            left.y + right.y,
            left.z + right.z
        };
    }

    AxisAlignedBox2D operator+(AxisAlignedBox2D left, Point2D right)
    {
        return
        {
            left.center + right,
            left.size
        };
    }
    
    AxisAlignedBox3D operator+(AxisAlignedBox3D left, Point3D right)
    {
        return
        {
            left.center + right,
            left.size
        };
    }

    Point2D operator-(Point2D left, Point2D right)
    {
        return
        {
            left.x - right.x,
            left.y - right.y
        };
    }

    Point3D operator-(Point3D left, Point3D right)
    {
        return
        {
            left.x - right.x,
            left.y - right.y,
            left.z - right.z
        };
    }

    AxisAlignedBox2D operator-(AxisAlignedBox2D left, Point2D right)
    {
        return
        {
            left.center - right,
            left.size
        };
    }

    AxisAlignedBox3D operator-(AxisAlignedBox3D left, Point3D right)
    {
        return
        {
            left.center - right,
            left.size
        };
    }

    Point2D ToPoint2D(Point3D point)
    {
        return
        {
            point.x,
            point.y
        };
    }

    Point2D ToPoint2D(Grid::Point point)
    {
        return
        {
            point.x * Grid::CellSize<float>,
            point.y * Grid::CellSize<float>
        };
    }

    Point2D ToPoint2D(Angle2D angle, Point2D::Value distance)
    {
        return
        {
            distance * cos(angle),
            distance * sin(angle)
        };
    }

    Point3D ToPoint3D(Point2D point, Point3D::Value z)
    {
        return
        {
            point.x,
            point.y,
            z
        };
    }

    Point3D ToPoint3D(Grid::Point point, Point3D::Value z)
    {
        return
        {
            point.x * Grid::CellSize<float>,
            point.y * Grid::CellSize<float>,
            z
        };
    }

    Point3D ToPoint3D(Angle3D angle, Point3D::Value distance)
    {
        return
        {
            distance * cos(angle.pitch) * sin(angle.yaw),
            distance * cos(angle.pitch) * cos(angle.yaw),
            distance * sin(angle.pitch)
        };
    }

    Angle2D ToDegrees(Angle2D angle)
    {
        const Angle2D halfCircleDegrees = 180;
        return halfCircleDegrees / (pi<Angle2D> * angle);
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
        Point::Value Distance(Point starting, Point destination)
        {
            return abs(destination.x - starting.x) + abs(destination.y - starting.y);
        }

        bool Contains(AxisAlignedBox box, Point point)
        {
            return point.x >= box.Left()
                && point.x <= box.Right()
                && point.y >= box.Top()
                && point.y <= box.Bottom();
        }

        bool Contains(AxisAlignedBox box, AxisAlignedBox other)
        {
            return other.Left() >= box.Left()
                && other.Right() <= box.Right()
                && other.Top() >= box.Top()
                && other.Bottom() <= box.Bottom();
        }

        bool Intersects(AxisAlignedBox one, AxisAlignedBox two)
        {
            return
                (one.Left() <= two.Right() && one.Right() >= two.Left()) &&
                (one.Top() <= two.Bottom() && one.Bottom() >= two.Top());
        }

        Point ToPoint(Point2D point)
        {
            return
            {
                static_cast<Point::Value>(std::floor(point.x / CellSize<Point2D::Value>)),
                static_cast<Point::Value>(std::floor(point.y / CellSize<Point2D::Value>))
            };
        }

        Point ToPoint(Point3D point)
        {
            return
            {
                static_cast<Point::Value>(std::floor(point.x / CellSize<Point3D::Value>)),
                static_cast<Point::Value>(std::floor(point.y / CellSize<Point3D::Value>))
            };
        }
    }
}