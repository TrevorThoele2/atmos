#include "FieldDestination.h"

#include <Inscription\Scribe.h>

namespace Atmos
{
    FieldDestination::FieldDestination(const Direction& direction, const GridPosition& position, FieldID id) :
        direction(direction), position(position), id(id)
    {}
}

namespace Inscription
{
    void Scribe<::Atmos::FieldDestination, BinaryArchive>::Scriven(ObjectT& object, ArchiveT& archive)
    {
        archive(object.direction);
        archive(object.position);
        archive(object.id);
    }
}