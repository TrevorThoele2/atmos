#include "Bounds.h"

namespace Atmos::Spatial
{
    Bounds::Bounds() = default;

    Bounds::Bounds(const Point3D& position, const Size2D& baseSize, const Scalers2D& scalers, const Angle2D& rotation) :
        position(position), baseSize(baseSize), scalers(scalers), rotation(rotation)
    {}

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
        return position;;
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
}

namespace Inscription
{
    void Scribe<Atmos::Spatial::Bounds, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        archive(object.position);
        archive(object.baseSize);
        archive(object.scalers);
        archive(object.rotation);
    }
}