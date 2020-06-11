#include "Bounds.h"

namespace Atmos
{
    Bounds::Bounds() = default;

    Bounds::Bounds(const Position3D& position, const Size2D& baseSize, const Scalers2D& scalers) :
        position(position), baseSize(baseSize), scalers(scalers)
    {}

    void Bounds::Position(const Position3D& to)
    {
        position = to;
    }

    void Bounds::BaseSize(const Size2D& to)
    {
        baseSize = to;
    }

    Position3D Bounds::Position() const
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

    void Bounds::Scalers(const Scalers2D& to)
    {
        scalers = to;
    }

    Scalers2D Bounds::Scalers() const
    {
        return scalers;
    }

    AxisAlignedBox2D Bounds::Box() const
    {
        return AxisAlignedBox2D(Position(), Size());
    }
}

namespace Inscription
{
    void Scribe<Atmos::Bounds, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        archive(object.position);
        archive(object.baseSize);
        archive(object.scalers);
    }
}