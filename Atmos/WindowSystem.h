#pragma once

#include "UniqueProviderSystem.h"
#include "WindowBase.h"

namespace Atmos
{
    class WindowSystem : public UniqueProviderSystem<WindowBase>
    {
    public:
        WindowSystem(ObjectManager& manager);
        WindowSystem(const ::Inscription::Table<WindowSystem>& table);
    private:
        void InitializeImpl() override;
    };
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DECLARE(::Atmos::WindowSystem)
    {
    public:
        INSCRIPTION_INSCRIPTER_DECLARE_TABLE;
        INSCRIPTION_DECLARE_CLASS_NAME_RESOLVER;
    };
}