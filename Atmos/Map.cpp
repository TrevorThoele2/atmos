#include "Map.h"

#include <Inscription/UnorderedSetScribe.h>

namespace Inscription
{
    void Scribe<Atmos::World::Map, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        archive(object.definedRegion);
    }
}