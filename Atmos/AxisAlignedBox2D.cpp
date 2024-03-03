#include "AxisAlignedBox2D.h"

namespace Atmos
{
    AxisAlignedBox2D::AxisAlignedBox2D(const Position2D& center, const Size2D& size) :
        center(center), size(size)
    {
        CalculateCoordinates();
    }

    AxisAlignedBox2D::AxisAlignedBox2D(Coordinate left, Coordinate top, Coordinate right, Coordinate bottom) :
        left(left), top(top), right(right), bottom(bottom)
    {
        CalculateCenterAndSize();
    }

    bool AxisAlignedBox2D::operator==(const AxisAlignedBox2D& arg) const
    {
        return left == arg.left && top == arg.top && right == arg.right && bottom == arg.bottom;
    }

    bool AxisAlignedBox2D::operator!=(const AxisAlignedBox2D& arg) const
    {
        return !(*this == arg);
    }

    void AxisAlignedBox2D::Center(const Position2D& to)
    {
        center = to;
        CalculateCoordinates();
    }

    void AxisAlignedBox2D::Size(const Size2D& to)
    {
        size = to;
        CalculateCoordinates();
    }

    Position2D AxisAlignedBox2D::Center() const
    {
        return center;
    }

    Size2D AxisAlignedBox2D::Size() const
    {
        return size;
    }

    void AxisAlignedBox2D::Edit(const Position2D& center, const Size2D& size)
    {
        this->center = center;
        this->size = size;
        CalculateCoordinates();
    }

    void AxisAlignedBox2D::Edit(Coordinate left, Coordinate top, Coordinate right, Coordinate bottom)
    {
        this->left = left;
        this->top = top;
        this->right = right;
        this->bottom = bottom;
        CalculateCenterAndSize();
    }

    void AxisAlignedBox2D::Left(Coordinate set)
    {
        ChangeCoordinate(left, set);
    }

    void AxisAlignedBox2D::Top(Coordinate set)
    {
        ChangeCoordinate(top, set);
    }

    void AxisAlignedBox2D::Right(Coordinate set)
    {
        ChangeCoordinate(right, set);
    }

    void AxisAlignedBox2D::Bottom(Coordinate set)
    {
        ChangeCoordinate(bottom, set);
    }

    auto AxisAlignedBox2D::Left() const -> Coordinate
    {
        return center.x - (size.width / 2);
    }

    auto AxisAlignedBox2D::Top() const -> Coordinate
    {
        return center.y - (size.height / 2);
    }

    auto AxisAlignedBox2D::Right() const -> Coordinate
    {
        return center.x + (size.width / 2);
    }

    auto AxisAlignedBox2D::Bottom() const -> Coordinate
    {
        return center.y + (size.height / 2);
    }

    Size2D::Value AxisAlignedBox2D::Width() const
    {
        return size.width;
    }

    Size2D::Value AxisAlignedBox2D::Height() const
    {
        return size.height;
    }

    bool AxisAlignedBox2D::Contains(const Position2D& check) const
    {
        return check.x >= left && check.x <= right && check.y >= top && check.y <= bottom;
    }

    bool AxisAlignedBox2D::Contains(const AxisAlignedBox2D& box) const
    {
        return left >= box.left && right <= box.right && top >= box.top && bottom <= box.bottom;
    }

    bool AxisAlignedBox2D::Intersects(const AxisAlignedBox2D& box) const
    {
        return
            (left <= box.right && right >= box.left) &&
            (top <= box.bottom && bottom >= box.top);
    }

    void AxisAlignedBox2D::ChangeCoordinate(Coordinate& change, Coordinate to)
    {
        change = to;
        CalculateCenterAndSize();
    }

    void AxisAlignedBox2D::CalculateCoordinates()
    {
        left = center.x - (size.width / 2);
        top = center.y - (size.height / 2);
        right = center.x + (size.width / 2);
        bottom  = center.y + (size.height / 2);
    }

    void AxisAlignedBox2D::CalculateCenterAndSize()
    {
        size.width = right - left;
        size.height = bottom - top;
        center.x = left + (size.width / 2);
        center.y = top + (size.height / 2);
    }
}

namespace Inscription
{
    void Scribe<::Atmos::AxisAlignedBox2D, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        archive(object.center);
        archive(object.size);
    }
}