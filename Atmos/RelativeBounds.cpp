#include "RelativeBounds.h"

namespace Inscription
{
    void Scribe<Atmos::Spatial::RelativeBounds, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        archive(object.delta);
    }
}