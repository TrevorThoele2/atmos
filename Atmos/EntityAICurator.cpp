#include "EntityAICurator.h"

#include <Arca/Reliquary.h>

#include "RunningScript.h"

namespace Atmos::Entity
{
    void AICurator::InitializeImplementation()
    {
        aiComponents = Owner().Batch<AIComponent>();
    }

    void AICurator::WorkImplementation(Stage& stage)
    {
        for (auto& loop : aiComponents)
        {
            auto running = loop.script->RunningForThis();
            if (!running)
                continue;

            running->Resume();
        }
    }
}