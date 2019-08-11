#pragma once

#include "UniqueProviderSystem.h"

#include <AGUI/Engine.h>

namespace Atmos
{
    class GuiSystem : public UniqueProviderSystem<Agui::Engine>
    {
    public:
        GuiSystem(ObjectManager& manager);
    protected:
        void InitializeImpl() override;
    };
}