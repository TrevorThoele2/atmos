#include "RelativeBoundsPosition.h"

namespace Inscription
{
    void Scribe<Atmos::Spatial::RelativeBoundsPosition, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        archive(object.delta);
    }
}