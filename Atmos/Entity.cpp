#include "Entity.h"

#include <Inscription/UnorderedMapScribe.h>
#include <Inscription/UnorderedSetScribe.h>

namespace Atmos::Entity
{
    Entity::Entity(Init init) : ClosedTypedRelic(init)
    {}

    Entity::Entity(
        Init init,
        Name name,
        Spatial::Grid::Point position,
        Spatial::Angle2D direction)
        :
        ClosedTypedRelic(init),
        name(name),
        position(position),
        direction(direction)
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
        archive(object.tags);
    }
}