#include "AxisAlignedBox2D.h"

namespace Atmos::Spatial
{
    AxisAlignedBox2D::AxisAlignedBox2D(const Point2D& center, const Size2D& size) :
        center(center), size(size)
    {}

    bool AxisAlignedBox2D::operator==(const AxisAlignedBox2D& arg) const
    {
        return center == arg.center && size == arg.size;
    }

    bool AxisAlignedBox2D::operator!=(const AxisAlignedBox2D& arg) const
    {
        return !(*this == arg);
    }

    auto AxisAlignedBox2D::Left() const -> Coordinate
    {
        return center.x - size.width / 2;
    }

    auto AxisAlignedBox2D::Top() const -> Coordinate
    {
        return center.y - size.height / 2;
    }

    auto AxisAlignedBox2D::Right() const -> Coordinate
    {
        return center.x + size.width / 2;
    }

    auto AxisAlignedBox2D::Bottom() const -> Coordinate
    {
        return center.y + size.height / 2;
    }
}