#include "AxisAlignedBox2D.h"

#include "MathUtility.h"

#include <Inscription\Scribe.h>

namespace Atmos
{
    AxisAlignedBox2D::AxisAlignedBox2D() :
        center(), size()
    {}

    AxisAlignedBox2D::AxisAlignedBox2D(Coordinate left, Coordinate top, Coordinate right, Coordinate bottom) :
        center((right - left) + left, (bottom - top) + top),
        size(right - left, bottom - top)
    {}

    AxisAlignedBox2D& AxisAlignedBox2D::operator=(const AxisAlignedBox2D& arg)
    {
        center = arg.center;
        size = arg.size;
        return *this;
    }

    bool AxisAlignedBox2D::operator==(const AxisAlignedBox2D& arg) const
    {
        return center == arg.center && size == arg.size;
    }

    bool AxisAlignedBox2D::operator!=(const AxisAlignedBox2D& arg) const
    {
        return !(*this == arg);
    }

    bool AxisAlignedBox2D::IsHit(const Position2D& check) const
    {
        return Within(check);
    }

    bool AxisAlignedBox2D::Within(const Position2D& check) const
    {
        return check.x >= left && check.x <= right && check.y >= top && check.y <= bottom;
    }

    bool AxisAlignedBox2D::Within(const AxisAlignedBox2D& box) const
    {
        return left >= box.left && right <= box.right && top >= box.top && bottom <= box.bottom;
    }

    bool AxisAlignedBox2D::Overlapping(const AxisAlignedBox2D& box) const
    {
        return
            (left <= box.right && right >= box.left) &&
            (top <= box.bottom && bottom >= box.top);
    }

    void AxisAlignedBox2D::SetCoordinate(Coordinate newValue, Coordinate focused, Position2D::Value centerValue, Size2D::Value sizeValue)
    {
        const auto delta = focused - newValue;
        sizeValue += delta;
        centerValue += (delta / 2);
    }

    AxisAlignedBox2D::Coordinate AxisAlignedBox2D::MakeMinusCoordinate(Position2D::Value centerValue, Size2D::Value sizeValue) const
    {
        return centerValue - (sizeValue / 2);
    }

    AxisAlignedBox2D::Coordinate AxisAlignedBox2D::MakePlusCoordinate(Position2D::Value centerValue, Size2D::Value sizeValue) const
    {
        return centerValue + (sizeValue / 2);
    }

    INSCRIPTION_BINARY_SERIALIZE_FUNCTION_DEFINE(AxisAlignedBox2D)
    {
        scribe(center);
        scribe(size);
    }
}