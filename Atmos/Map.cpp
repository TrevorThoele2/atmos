#include "Map.h"

#include <Inscription/UnorderedSetScribe.h>
#include <Inscription/VectorScribe.h>

namespace Inscription
{
    void Scribe<Atmos::World::Map, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        archive(object.terrain);
        archive(object.entityBoundary);
    }
}