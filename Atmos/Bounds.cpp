#include "Bounds.h"

namespace Atmos::Spatial
{
    Bounds::Bounds() = default;

    Bounds::Bounds(
        Spatial::Space space, const Point3D& position, const Size2D& baseSize, const Scalers2D& scalers, const Angle2D& rotation)
        :
        space(space), position(position), baseSize(baseSize), scalers(scalers), rotation(rotation)
    {}

    bool Bounds::operator==(const Bounds& arg) const
    {
        return space == arg.space
            && position == arg.position
            && baseSize == arg.baseSize
            && scalers == arg.scalers
            && rotation == arg.rotation;
    }

    bool Bounds::operator!=(const Bounds& arg) const
    {
        return !(*this == arg);
    }

    void Bounds::Position(const Point3D& to)
    {
        position = to;
    }

    void Bounds::BaseSize(const Size2D& to)
    {
        baseSize = to;
    }

    void Bounds::Scalers(const Scalers2D& to)
    {
        scalers = to;
    }

    void Bounds::Rotation(const Angle2D& to)
    {
        rotation = to;
    }

    Point3D Bounds::Position() const
    {
        return position;
    }

    Size2D Bounds::BaseSize() const
    {
        return baseSize;
    }

    Size2D Bounds::Size() const
    {
        return Size2D{ baseSize.width * scalers.x, baseSize.height * scalers.y };
    }

    Scalers2D Bounds::Scalers() const
    {
        return scalers;
    }

    Angle2D Bounds::Rotation() const
    {
        return rotation;
    }

    Space Bounds::Space() const
    {
        return space;
    }
}