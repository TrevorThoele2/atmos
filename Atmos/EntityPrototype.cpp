#include "EntityPrototype.h"

namespace Atmos::Entity
{
    Prototype::Prototype(Init init) : ClosedTypedRelic(init)
    {}

    Prototype::Prototype(
        Init init,
        Arca::Index<Script::Instance> initializer,
        Name name,
        Spatial::Grid::Point position,
        Spatial::Angle2D direction)
        :
        ClosedTypedRelic(init),
        initializer(initializer),
        name(name),
        position(position),
        direction(direction)
    {}
}

namespace Inscription
{
    void Scribe<Atmos::Entity::Prototype, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        archive(object.initializer);
        archive(object.name);
        archive(object.position);
        archive(object.direction);
    }
}