#include "GridAxisAlignedBox.h"

namespace Atmos::Spatial::Grid
{
    AxisAlignedBox::AxisAlignedBox(const Point& center, const Size& size) :
        center(center), size(size)
    {}

    bool AxisAlignedBox::operator==(const AxisAlignedBox& arg) const
    {
        return center == arg.center && size == arg.size;
    }

    bool AxisAlignedBox::operator!=(const AxisAlignedBox& arg) const
    {
        return !(*this == arg);
    }

    auto AxisAlignedBox::Left() const -> Coordinate
    {
        return center.x - size.width / 2;
    }

    auto AxisAlignedBox::Top() const -> Coordinate
    {
        return center.y - size.height / 2;
    }

    auto AxisAlignedBox::Right() const -> Coordinate
    {
        return center.x + size.width / 2;
    }

    auto AxisAlignedBox::Bottom() const -> Coordinate
    {
        return center.y + size.height / 2;
    }
}