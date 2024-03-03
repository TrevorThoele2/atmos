#include "EntityDatum.h"

namespace Inscription
{
    void Scribe<Atmos::Entity::Datum, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        archive(object.name);
        archive(object.value);
    }
}