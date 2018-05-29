#pragma once

#include "EntityComponent.h"

#include "Script.h"

namespace Atmos
{
    namespace Ent
    {
        class DialogueComponent : public Component<DialogueComponent>
        {
        private:
            INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
            INSCRIPTION_ACCESS;

            void OnOwnerEntitySet() override final;
            
            void SetScriptCallers();
        public:
            Script::Instance script;
            DialogueComponent() = default;
            DialogueComponent(const DialogueComponent &arg) = default;
            DialogueComponent(DialogueComponent &&arg);
            DialogueComponent& operator=(const DialogueComponent &arg) = default;
            DialogueComponent& operator=(DialogueComponent &&arg);
        };

        ENTITY_COMPONENT_MAP_DECLARE("Dialogue", DialogueComponent)
    }
}