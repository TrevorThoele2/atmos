#include "Bounds.h"

namespace Atmos
{
    void Bounds::Position(const Position3D& to)
    {
        position = to;
        CalculateBox();
    }

    void Bounds::Size(const Size3D& to)
    {
        size = to;
        CalculateBox();
    }

    Position3D Bounds::Position() const
    {
        return position;
    }

    Size3D Bounds::Size() const
    {
        return size;
    }

    AxisAlignedBox3D Bounds::Box() const
    {
        return box;
    }

    void Bounds::CalculateBox()
    {
        
    }
}

namespace Arca
{
    const TypeName Traits<Atmos::Bounds>::typeName = "Bounds";
}

namespace Inscription
{
    void Scribe<Atmos::Bounds, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        archive(object.position);
        archive(object.size);
        if (archive.IsInput())
            object.CalculateBox();
    }
}