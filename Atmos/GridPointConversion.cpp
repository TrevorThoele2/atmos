#include "GridPointConversion.h"

#include "GridCellSize.h"

namespace Atmos::Spatial::Grid
{
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