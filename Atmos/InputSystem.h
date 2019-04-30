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
            System(const ::Inscription::Table<System>& table);
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
        INSCRIPTION_INSCRIPTER_DECLARE_TABLE;
        INSCRIPTION_DECLARE_CLASS_NAME_RESOLVER;
    };
}