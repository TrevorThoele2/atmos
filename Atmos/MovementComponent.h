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
        public:
            ::affecter::Connection affecterConnection;

            Script::Instance upMod;
            Script::Instance downMod;
            Script::Instance leftMod;
            Script::Instance rightMod;

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