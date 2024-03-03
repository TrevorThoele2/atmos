#pragma once

#include <list>
#include "EntityComponent.h"
#include "Script.h"

#include "Serialization.h"

namespace Atmos
{
    namespace Ent
    {
        class AIComponent : public Component<AIComponent>
        {
        private:
            INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
            INSCRIPTION_ACCESS;
        public:
            Script::Instance ai;
            Script::Instance battleAI;

            AIComponent() = default;
            AIComponent(const AIComponent &arg) = default;
            AIComponent(AIComponent &&arg);
            AIComponent& operator=(const AIComponent &arg) = default;
            AIComponent& operator=(AIComponent &&arg);
        };

        ENTITY_COMPONENT_MAP_DECLARE("AI", AIComponent)
    }
}