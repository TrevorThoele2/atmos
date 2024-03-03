#include "EntityCurator.h"

#include <Arca/Reliquary.h>
#include <Arca/Created.h>
#include <Arca/Destroying.h>

namespace Atmos::Entity
{
    EntityCurator::EntityCurator(Init init) :
        Curator(init), mappedEntities(init.owner)
    {
        Owner().On<Arca::CreatedKnown<Entity>>(
            [](const Arca::CreatedKnown<Entity>& signal)
            {
                
            });

        Owner().On<Arca::DestroyingKnown<Entity>>(
            [](const Arca::DestroyingKnown<Entity>& signal)
            {

            });
    }
}