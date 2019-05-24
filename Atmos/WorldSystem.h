#pragma once

#include "UnownedProviderSystem.h"
#include "WorldManager.h"

namespace Atmos
{
    class WorldSystem : public UnownedProviderSystem<WorldManager>
    {
    public:
        WorldSystem(ObjectManager& manager);
        INSCRIPTION_BINARY_TABLE_CONSTRUCTOR_DECLARE(WorldSystem);
    };
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DECLARE(::Atmos::WorldSystem)
    {
    public:
        INSCRIPTION_BINARY_INSCRIPTER_DECLARE_TABLE;
        INSCRIPTION_BINARY_DECLARE_CLASS_NAME_RESOLVER;
    };
}
