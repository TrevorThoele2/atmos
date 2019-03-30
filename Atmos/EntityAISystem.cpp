
#include "EntityAISystem.h"

#include "RunningScript.h"

#include "ObjectManager.h"

namespace Atmos
{
    namespace Ent
    {
        nEntityAISystem::nEntityAISystem(ObjectManager& manager) : ObjectSystem(manager)
        {
            aiComponents = manager.Batch<nAIComponent>();
        }

        void nEntityAISystem::WorkImpl()
        {
            for (auto& loop : aiComponents)
            {
                auto running = loop->script->RunningForThis();
                if (!running.IsOccupied())
                    continue;

                running->Resume();
            }
        }
    }
}