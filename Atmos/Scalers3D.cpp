#include "Scalers3D.h"

namespace Atmos::Spatial
{
    bool Scalers3D::operator==(const Scalers3D& arg) const
    {
        return x == arg.x && y == arg.y && z == arg.z;
    }

    bool Scalers3D::operator!=(const Scalers3D& arg) const
    {
        return !(*this == arg);
    }
}

namespace Inscription
{
    void Scribe<Atmos::Spatial::Scalers3D, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        archive(object.x);
        archive(object.y);
        archive(object.z);
    }
}