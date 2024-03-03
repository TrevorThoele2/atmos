#pragma once

#include "UnownedProviderSystem.h"

#include "Engine.h"

#include "Event.h"

namespace Atmos
{
    class EngineSystem : public UnownedProviderSystem<Engine>
    {
    public:
        Event<> onFocusLost;
        Event<> onFocusRegained;
    public:
        EngineSystem(ObjectManager& manager);
        INSCRIPTION_BINARY_TABLE_CONSTRUCTOR_DECLARE(EngineSystem);
    };
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DECLARE(::Atmos::EngineSystem)
    {
    public:
        INSCRIPTION_BINARY_INSCRIPTER_DECLARE_TABLE;
        INSCRIPTION_BINARY_DECLARE_CLASS_NAME_RESOLVER;
    };
}