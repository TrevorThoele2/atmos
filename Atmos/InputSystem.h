#pragma once

#include "UniqueProviderSystem.h"
#include "InputManager.h"

namespace Atmos
{
    namespace Input
    {
        class System : public UniqueProviderSystem<Manager>
        {
        public:
            System(ObjectManager& manager);
            INSCRIPTION_BINARY_TABLE_CONSTRUCTOR_DECLARE(System);
        private:
            void InitializeImpl() override;
        };
    }
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DECLARE(::Atmos::Input::System)
    {
    public:
        INSCRIPTION_BINARY_INSCRIPTER_DECLARE_TABLE;
        INSCRIPTION_BINARY_DECLARE_CLASS_NAME_RESOLVER;
    };
}