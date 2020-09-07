#include "Point3D.h"

#include <Inscription/Scribe.h>

namespace Atmos::Spatial
{
    bool Point3D::operator==(const Point3D& arg) const
    {
        return x == arg.x && y == arg.y && z == arg.z;
    }

    bool Point3D::operator!=(const Point3D& arg) const
    {
        return !(*this == arg);
    }
}

namespace Inscription
{
    void Scribe<Atmos::Spatial::Point3D, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        archive(object.x);
        archive(object.y);
        archive(object.z);
    }
}