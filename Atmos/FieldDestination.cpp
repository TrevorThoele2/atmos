#include "FieldDestination.h"

#include <Inscription/Scribe.h>

namespace Inscription
{
    void Scribe<::Atmos::World::FieldDestination, BinaryArchive>::ScrivenImplementation(
        ObjectT& object, ArchiveT& archive)
    {
        archive(object.direction);
        archive(object.position);
        archive(object.id);
    }
}