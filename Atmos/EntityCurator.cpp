#include "EntityCurator.h"

#include <Arca/Reliquary.h>
#include <Arca/Created.h>
#include <Arca/Destroying.h>
#include <Arca/Actualization.h>

#include "DebugStatistics.h"

namespace Atmos::Entity
{
    EntityCurator::EntityCurator(Init init) :
        Curator(init), mappedEntities(init.owner), debugStatistics(init.owner)
    {
        Owner().ExecuteOn<Arca::CreatedKnown<Entity>>(
            [](const Arca::CreatedKnown<Entity>& signal)
            {
                
            });

        Owner().ExecuteOn<Arca::DestroyingKnown<Entity>>(
            [](const Arca::DestroyingKnown<Entity>& signal)
            {

            });
    }
}