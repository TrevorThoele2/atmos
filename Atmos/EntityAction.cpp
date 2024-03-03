#include "EntityAction.h"

namespace Inscription
{
    void Scribe<Atmos::Entity::Action, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        archive(object.activation);
        archive(object.script);
    }
}