#pragma once

#include "UnownedProviderSystem.h"

#include "Engine.h"

#include "Event.h"

namespace Atmos
{
    class EngineSystem : public UnownedProviderSystem<Engine>
    {
    public:
        NullEvent onFocusLost;
        NullEvent onFocusRegained;
    public:
        EngineSystem(ObjectManager& manager);
        EngineSystem(const ::Inscription::Table<EngineSystem>& table);
    };
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DECLARE(::Atmos::EngineSystem)
    {
    public:
        INSCRIPTION_INSCRIPTER_DECLARE_TABLE;
        INSCRIPTION_DECLARE_CLASS_NAME_RESOLVER;
    };
}