#include "Map.h"

#include <Inscription/UnorderedSetScribe.h>

namespace Arca
{
    const TypeName Traits<Atmos::World::Map>::typeName = "Map";
}

namespace Inscription
{
    void Scribe<Atmos::World::Map, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        archive(object.definedRegion);
    }
}