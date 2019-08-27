#include "AxisAlignedBox3D.h"

#include <cctype>
#include <Chroma/Contract.h>

namespace Atmos
{
    AxisAlignedBox3D::AxisAlignedBox3D(const Position3D& center, const Size3D& size) :
        center(center), size(size)
    {
        CalculateCoordinates();
    }

    AxisAlignedBox3D::AxisAlignedBox3D(
        Coordinate left,
        Coordinate top,
        Coordinate farZ,
        Coordinate right,
        Coordinate bottom,
        Coordinate nearZ
    ) :
        left(left), top(top), farZ(farZ), right(right), bottom(bottom), nearZ(nearZ)
    {
        CoordinatePrecondition(left, right, "left", "right");
        CoordinatePrecondition(top, bottom, "top", "bottom");
        CoordinatePrecondition(farZ, nearZ, "farZ", "nearZ");
        CalculateCenterAndSize();
    }

    bool AxisAlignedBox3D::operator==(const AxisAlignedBox3D& arg) const
    {
        return center == arg.center && size == arg.size;
    }

    bool AxisAlignedBox3D::operator!=(const AxisAlignedBox3D& arg) const
    {
        return !(*this == arg);
    }

    void AxisAlignedBox3D::Center(const Position3D& to)
    {
        center = to;
        CalculateCoordinates();
    }

    void AxisAlignedBox3D::Size(const Size3D& to)
    {
        size = to;
        CalculateCoordinates();
    }

    Position3D AxisAlignedBox3D::Center() const
    {
        return center;
    }

    Size3D AxisAlignedBox3D::Size() const
    {
        return size;
    }

    void AxisAlignedBox3D::Edit(const Position3D& center, const Size3D& size)
    {
        this->center = center;
        this->size = size;
        CalculateCoordinates();
    }

    void AxisAlignedBox3D::Edit(
        Coordinate left,
        Coordinate top,
        Coordinate farZ,
        Coordinate right,
        Coordinate bottom,
        Coordinate nearZ
    ) {
        CoordinatePrecondition(left, right, "left", "right");
        CoordinatePrecondition(top, bottom, "top", "bottom");
        CoordinatePrecondition(farZ, nearZ, "farZ", "nearZ");
        this->left = left;
        this->top = top;
        this->farZ = farZ;
        this->right = right;
        this->bottom = bottom;
        this->nearZ = nearZ;
        CalculateCenterAndSize();
    }

    void AxisAlignedBox3D::Left(Coordinate set)
    {
        CoordinatePrecondition(set, right, "left", "right");
        ChangeCoordinate(left, set);
    }

    void AxisAlignedBox3D::Top(Coordinate set)
    {
        CoordinatePrecondition(set, bottom, "top", "bottom");
        ChangeCoordinate(top, set);
    }

    void AxisAlignedBox3D::FarZ(Coordinate set)
    {
        CoordinatePrecondition(set, nearZ, "farZ", "nearZ");
        ChangeCoordinate(farZ, set);
    }

    void AxisAlignedBox3D::Right(Coordinate set)
    {
        CoordinatePrecondition(left, set, "left", "right");
        ChangeCoordinate(right, set);
    }

    void AxisAlignedBox3D::Bottom(Coordinate set)
    {
        CoordinatePrecondition(top, set, "top", "bottom");
        ChangeCoordinate(bottom, set);
    }

    void AxisAlignedBox3D::NearZ(Coordinate set)
    {
        CoordinatePrecondition(farZ, set, "farZ", "nearZ");
        ChangeCoordinate(nearZ, set);
    }

    auto AxisAlignedBox3D::Left() const -> Coordinate
    {
        return left;
    }

    auto AxisAlignedBox3D::Top() const -> Coordinate
    {
        return top;
    }

    auto AxisAlignedBox3D::FarZ() const -> Coordinate
    {
        return farZ;
    }

    auto AxisAlignedBox3D::Right() const -> Coordinate
    {
        return right;
    }

    auto AxisAlignedBox3D::Bottom() const -> Coordinate
    {
        return bottom;
    }

    auto AxisAlignedBox3D::NearZ() const -> Coordinate
    {
        return nearZ;
    }

    Size3D::Value AxisAlignedBox3D::Width() const
    {
        return size.width;
    }

    Size3D::Value AxisAlignedBox3D::Height() const
    {
        return size.height;
    }

    Size3D::Value AxisAlignedBox3D::Depth() const
    {
        return size.depth;
    }

    bool AxisAlignedBox3D::Contains(const Position3D& check) const
    {
        return
            check.x >= left &&
            check.x <= right &&
            check.y >= top &&
            check.y <= bottom &&
            check.z >= farZ &&
            check.z <= nearZ;
    }

    bool AxisAlignedBox3D::Contains(const AxisAlignedBox3D& box) const
    {
        return
            left >= box.left &&
            right <= box.right &&
            top >= box.top &&
            bottom <= box.bottom &&
            farZ >= box.farZ &&
            farZ <= box.nearZ;
    }

    bool AxisAlignedBox3D::Intersects(const AxisAlignedBox3D& box) const
    {
        return
            (left <= box.right && right >= box.left) &&
            (top <= box.bottom && bottom >= box.top) &&
            (farZ <= box.nearZ && nearZ >= box.farZ);
    }

    void AxisAlignedBox3D::ChangeCoordinate(Coordinate& change, Coordinate to)
    {
        change = to;
        CalculateCenterAndSize();
    }

    void AxisAlignedBox3D::CalculateCoordinates()
    {
        left = center.x - (size.width / 2);
        top = center.y - (size.height / 2);
        farZ = center.z - (size.depth / 2);
        right = center.x + (size.width / 2);
        bottom = center.y + (size.height / 2);
        nearZ = center.z + (size.depth / 2);
    }

    void AxisAlignedBox3D::CalculateCenterAndSize()
    {
        size.width = right - left;
        size.height = bottom - top;
        size.depth = nearZ - farZ;
        center.x = left + (size.width / 2);
        center.y = top + (size.height / 2);
        center.z = farZ + (size.depth / 2);
    }

    void AxisAlignedBox3D::CoordinatePrecondition(
        const Coordinate& low,
        const Coordinate& high,
        const std::string& lowName,
        const std::string& highName
    ) {
        auto useLowName = lowName;
        useLowName[0] = std::toupper(useLowName[0]);

        SOFT_PRECONDITION(
            low <= high,
            std::invalid_argument(useLowName + " must be <= " + highName + " when given to an AxisAlignedBox3D.")
        );
    }
}

namespace Inscription
{
    void Scribe<::Atmos::AxisAlignedBox3D, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        archive(object.center);
        archive(object.size);
    }
}