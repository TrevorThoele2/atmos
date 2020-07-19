#include "Angle3D.h"

namespace Inscription
{
    void Scribe<Atmos::Spatial::Angle3D, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        archive(object.yaw);
        archive(object.pitch);
    }
}