#pragma once

#include "EntityComponent.h"
#include "Currency.h"
#include "Serialization.h"

namespace Atmos
{
    namespace Ent
    {
        class AvatarComponent : public Component<AvatarComponent>
        {
        private:
            INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
            INSCRIPTION_ACCESS;
        public:
            typedef Atmos::Gold Gold;
            Gold gold;

            AvatarComponent();
            AvatarComponent(const AvatarComponent &arg) = default;
            AvatarComponent(AvatarComponent &&arg);
            AvatarComponent& operator=(const AvatarComponent &arg) = default;
            AvatarComponent& operator=(AvatarComponent &&arg);
        };

        ENTITY_COMPONENT_MAP_DECLARE("Avatar", AvatarComponent)
    }
}