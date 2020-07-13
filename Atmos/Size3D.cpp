#include "Size3D.h"

namespace Atmos::Spatial
{
    bool Size3D::operator==(const Size3D& arg) const
    {
        return width == arg.width && height == arg.height && depth == arg.depth;
    }

    bool Size3D::operator!=(const Size3D& arg) const
    {
        return !(*this == arg);
    }
}

namespace Inscription
{
    void Scribe<Atmos::Spatial::Size3D, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        archive(object.width);
        archive(object.height);
        archive(object.depth);
    }
}