#include "Entity.h"

#include <Inscription/UnorderedMapScribe.h>
#include <Inscription/UnorderedSetScribe.h>

namespace Atmos::Entity
{
    Entity::Entity(Init init) : ClosedTypedRelic(init)
    {}
}

namespace Inscription
{
    void Scribe<Atmos::Entity::Entity, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        archive(object.name);
        archive(object.displayName);
        archive(object.position);
        archive(object.solid);
        archive(object.data);
        archive(object.actions);
        archive(object.tags);
    }
}