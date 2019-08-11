#include "FieldDestination.h"

#include <Inscription\Scribe.h>

namespace Atmos::World
{
    FieldDestination::FieldDestination(const Direction& direction, const Grid::Position& position, FieldID id) :
        direction(direction), position(position), id(id)
    {}
}

namespace Inscription
{
    void Scribe<::Atmos::World::FieldDestination, BinaryArchive>::ScrivenImplementation(
        ObjectT& object, ArchiveT& archive)
    {
        archive(object.direction);
        archive(object.position);
        archive(object.id);
    }

    void Scribe<::Atmos::World::FieldDestination, BinaryArchive>::ConstructImplementation(
        ObjectT* storage, ArchiveT& archive)
    {
        DoBasicConstruction(storage, archive);
    }
}