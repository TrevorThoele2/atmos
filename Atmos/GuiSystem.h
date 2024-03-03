#pragma once

#include "UniqueProviderSystem.h"

#include <AGUI/Engine.h>

namespace Atmos
{
    class GuiSystem : public UniqueProviderSystem<Agui::Engine>
    {
    public:
        GuiSystem(ObjectManager& manager);
    private:
        void InitializeImpl() override;
    };
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DECLARE(::Atmos::GuiSystem)
    {
    public:
        INSCRIPTION_BINARY_INSCRIPTER_DECLARE_SERIALIZE_FUNCTION;
        INSCRIPTION_BINARY_INSCRIPTER_DECLARE_CONSTRUCT_OBJECT_FUNCTION;

        INSCRIPTION_BINARY_DECLARE_CLASS_NAME_RESOLVER;
    };
}