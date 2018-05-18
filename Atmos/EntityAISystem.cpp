
#include <cassert>
#include "EntityAISystem.h"

#include "MainGame.h"
#include "Battle.h"
#include "WorldManager.h"

#include "CurrentField.h"

namespace Atmos
{
    namespace Ent
    {
        AISystem::ScriptToUse AISystem::DetermineScriptToUse()
        {
            if (mainGameState.IsTop())
                return &AIComponent::ai;
            else if (battleState.IsTop())
                return &AIComponent::battleAI;
                
            return nullptr;
        }

        void AISystem::Work()
        {
            ScriptToUse use = DetermineScriptToUse();
            if (!use)
                return;

            for (auto &loop : GetCurrentEntities()->GetRawMap<AIComponent>())
            {
                if ((loop.second.*use).IsValid())
                   (loop.second.*use).Resume();
            }
        }

        ENTITY_SYSTEM_FORCE_INSTANTIATION(AISystem);
    }
}