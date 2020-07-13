#include "PointConversions.h"

#include "GridCellSize.h"

namespace Atmos::Spatial
{
    Point2D ToPoint2D(const Point3D& position)
    {
        return
        {
            position.x,
            position.y
        };
    }

    Point3D ToPoint3D(const Grid::Point& position, Point3D::Value z)
    {
        return
        {
            position.x * Grid::CellSize<float>,
            position.y * Grid::CellSize<float>,
            z
        };
    }
}