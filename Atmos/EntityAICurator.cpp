#include "EntityAICurator.h"

#include <Arca/Reliquary.h>

#include "RunningScript.h"

namespace Atmos::Entity
{
    AICurator::AICurator(Init init) :
        Curator(init), aiComponents(init.owner.Batch<AIComponent>())
    {}

    void AICurator::Work()
    {
        for (auto& loop : aiComponents)
        {
            const auto running = loop.script->RunningForThis();
            if (!running)
                continue;

            Data(running)->Resume();
        }
    }
}