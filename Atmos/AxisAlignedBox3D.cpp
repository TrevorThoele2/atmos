#include "AxisAlignedBox3D.h"

namespace Atmos::Spatial
{
    AxisAlignedBox3D::AxisAlignedBox3D(const Point3D& center, const Size3D& size) :
        center(center), size(size)
    {}

    bool AxisAlignedBox3D::operator==(const AxisAlignedBox3D& arg) const
    {
        return center == arg.center && size == arg.size;
    }

    bool AxisAlignedBox3D::operator!=(const AxisAlignedBox3D& arg) const
    {
        return !(*this == arg);
    }

    auto AxisAlignedBox3D::Left() const -> Coordinate
    {
        return center.x - size.width / 2;
    }

    auto AxisAlignedBox3D::Top() const -> Coordinate
    {
        return center.y - size.height / 2;
    }

    auto AxisAlignedBox3D::FarZ() const -> Coordinate
    {
        return center.z - size.depth / 2;
    }

    auto AxisAlignedBox3D::Right() const -> Coordinate
    {
        return center.x + size.width / 2;
    }

    auto AxisAlignedBox3D::Bottom() const -> Coordinate
    {
        return center.y + size.height / 2;
    }

    auto AxisAlignedBox3D::NearZ() const -> Coordinate
    {
        return center.z + size.depth / 2;
    }
}