#pragma once

#include "UnownedProviderSystem.h"
#include "WorldManager.h"

namespace Atmos
{
    class WorldSystem : public UnownedProviderSystem<WorldManager>
    {
    public:
        WorldSystem(ObjectManager& manager);
        WorldSystem(const ::Inscription::Table<WorldSystem>& table);
    };
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DECLARE(::Atmos::WorldSystem)
    {
    public:
        INSCRIPTION_INSCRIPTER_DECLARE_TABLE;
        INSCRIPTION_DECLARE_CLASS_NAME_RESOLVER;
    };
}
