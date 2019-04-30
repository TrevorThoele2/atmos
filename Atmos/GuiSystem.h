#pragma once

#include "ObjectSystem.h"

namespace Atmos
{
    class GuiSystem : public ObjectSystem
    {
    public:
        GuiSystem(ObjectManager& manager);
        GuiSystem(const ::Inscription::Table<GuiSystem>& table);
    private:
        void InitializeImpl() override;
    };
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DECLARE(::Atmos::GuiSystem)
    {
    public:
        INSCRIPTION_INSCRIPTER_DECLARE_TABLE;
        INSCRIPTION_DECLARE_CLASS_NAME_RESOLVER;
    };
}