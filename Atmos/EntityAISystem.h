#pragma once

#include <vector>
#include "EntitySystem.h"
#include "AIComponent.h"

#include "Script.h"

namespace Atmos
{
    namespace Ent
    {
        class AISystem : public System<AISystem>
        {
        private:
            typedef System<AISystem> SystemBaseT;
            friend SystemBaseT;

            typedef Script::Instance(AIComponent::*ScriptToUse);

            AISystem() = default;
            AISystem(const AISystem &arg) = delete;
            AISystem& operator=(const AISystem &arg) = delete;

            static ScriptToUse DetermineScriptToUse();
        public:
            static void Work();
        };

        template class System<AISystem>;
    }
}