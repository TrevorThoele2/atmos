#include "AxisAlignedBox3D.h"

#include "MathUtility.h"

#include <Inscription\Scribe.h>

namespace Atmos
{
    AxisAlignedBox3D::AxisAlignedBox3D() :
        center(), size()
    {}

    AxisAlignedBox3D::AxisAlignedBox3D(Coordinate left, Coordinate top, Coordinate farZ, Coordinate right, Coordinate bottom, Coordinate nearZ) :
        center((right - left) + left, (bottom - top) + top, (farZ - nearZ) + farZ),
        size(right - left, bottom - top, farZ - nearZ)
    {}

    AxisAlignedBox3D& AxisAlignedBox3D::operator=(const AxisAlignedBox3D& arg)
    {
        center = arg.center;
        size = arg.size;
        return *this;
    }

    bool AxisAlignedBox3D::operator==(const AxisAlignedBox3D& arg) const
    {
        return center == arg.center && size == arg.size;
    }

    bool AxisAlignedBox3D::operator!=(const AxisAlignedBox3D& arg) const
    {
        return !(*this == arg);
    }

    bool AxisAlignedBox3D::IsHit(const Position3D& check) const
    {
        return Within(check);
    }

    bool AxisAlignedBox3D::Within(const Position3D& check) const
    {
        return check.x >= left && check.x <= right && check.y >= top && check.y <= bottom && check.z >= farZ && check.z <= nearZ;
    }

    bool AxisAlignedBox3D::Within(const AxisAlignedBox3D& box) const
    {
        return left >= box.left && right <= box.right && top >= box.top && bottom <= box.bottom && farZ >= box.farZ && farZ <= box.farZ;
    }

    bool AxisAlignedBox3D::Overlapping(const AxisAlignedBox3D& box) const
    {
        return
            (left <= box.right && right >= box.left) &&
            (top <= box.bottom && bottom >= box.top) &&
            (farZ <= box.nearZ && nearZ >= box.farZ);
    }

    void AxisAlignedBox3D::SetCoordinate(Coordinate newValue, Coordinate focused, Position3D::Value centerValue, Size3D::Value sizeValue)
    {
        const auto delta = focused - newValue;
        sizeValue += delta;
        centerValue += (delta / 2);
    }

    AxisAlignedBox3D::Coordinate AxisAlignedBox3D::MakeMinusCoordinate(Position3D::Value centerValue, Size3D::Value sizeValue) const
    {
        return centerValue - (sizeValue / 2);
    }

    AxisAlignedBox3D::Coordinate AxisAlignedBox3D::MakePlusCoordinate(Position3D::Value centerValue, Size3D::Value sizeValue) const
    {
        return centerValue + (sizeValue / 2);
    }
}

namespace Inscription
{
    void Scribe<::Atmos::AxisAlignedBox3D, BinaryArchive>::Scriven(ObjectT& object, ArchiveT& archive)
    {
        archive(object.center);
        archive(object.size);
    }
}