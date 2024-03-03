#pragma once

#include "UniqueProviderSystem.h"
#include "WindowBase.h"

namespace Atmos
{
    class WindowSystem : public UniqueProviderSystem<WindowBase>
    {
    public:
        WindowSystem(ObjectManager& manager);
        INSCRIPTION_BINARY_TABLE_CONSTRUCTOR_DECLARE(WindowSystem);
    private:
        void InitializeImpl() override;
    };
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DECLARE(::Atmos::WindowSystem)
    {
    public:
        INSCRIPTION_BINARY_INSCRIPTER_DECLARE_TABLE;
        INSCRIPTION_BINARY_DECLARE_CLASS_NAME_RESOLVER;
    };
}