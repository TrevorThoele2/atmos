#include "EntityCurator.h"

#include <Arca/Reliquary.h>
#include <Arca/Created.h>
#include <Arca/Destroying.h>
#include <Arca/Actualization.h>

#include "DebugStatistics.h"

namespace Atmos::Entity
{
    void EntityCurator::InitializeImplementation()
    {
        Owner().ExecuteOn<Arca::Created>(
            [](const Arca::Created& signal)
            {
                auto actualized = Arca::Actualize<Entity>(signal.handle);
                if (!actualized)
                    return;
            });

        Owner().ExecuteOn<Arca::Destroying>(
            [](const Arca::Destroying& signal)
            {
                auto actualized = Arca::Actualize<Entity>(signal.handle);
                if (!actualized)
                    return;
            });

        mappedEntities = Arca::GlobalPtr<MappedEntities>(Owner());

        debugStatistics = Arca::GlobalPtr<Debug::Statistics>(Owner());
    }
}