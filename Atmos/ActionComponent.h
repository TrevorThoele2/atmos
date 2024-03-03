#pragma once

#include "EntityComponent.h"
#include "Action.h"

#include "Serialization.h"

namespace Atmos
{
    namespace Ent
    {
        class ActionComponent : public Component<ActionComponent>
        {
        public:
            enum class Activator : unsigned short
            {
                ENTER_TILE,
                USE_ON,
                ENTER_FIELD
            };
        private:
            INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
            INSCRIPTION_ACCESS;
        public:
            Activator activator;
            Act::Action act;

            ActionComponent();
            ActionComponent(const ActionComponent &arg) = default;
            ActionComponent(ActionComponent &&arg);
            ActionComponent& operator=(const ActionComponent &arg) = default;
            ActionComponent& operator=(ActionComponent &&arg);

            void OnMovedInto();
            void OnUseOnTop();
            void OnFieldEntered();
        };

        ENTITY_COMPONENT_MAP_DECLARE("Action", ActionComponent)
    }
}