#pragma once

#include <vector>
#include "EntityComponent.h"

#include "Name.h"
#include "Script.h"

#include <Affecter/Affecter.h>

#include "Serialization.h"

namespace Atmos
{
    namespace Ent
    {
        class MovementComponent : public Component<MovementComponent>
        {
        private:
            INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
            INSCRIPTION_ACCESS;

            void OnOwnerEntitySet() override final;

            void SetScriptCallers();
        public:
            ::affecter::Connection affecterConnection;

            Script::Instance movementMod;
            Script::Instance changeDirectionMod;

            MovementComponent() = default;
            MovementComponent(const MovementComponent &arg);
            MovementComponent(MovementComponent &&arg);
            MovementComponent& operator=(const MovementComponent &arg);
            MovementComponent& operator=(MovementComponent &&arg);

            bool IsMoving() const;
            bool CanMove() const;
        };

        ENTITY_COMPONENT_MAP_DECLARE("Movement", MovementComponent)
    }
}