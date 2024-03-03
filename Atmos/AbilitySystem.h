#pragma once

#include "RegistryObjectSystem.h"

#include "Ability.h"

#include "Serialization.h"

namespace Atmos
{
    class AbilitySystem : public RegistryObjectSystem<nAbility>
    {
    public:
        AbilitySystem(ObjectManager& manager);
        AbilitySystem(const ::Inscription::Table<AbilitySystem>& table);
    };
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DECLARE(::Atmos::AbilitySystem)
    {
    public:
        INSCRIPTION_INSCRIPTER_DECLARE_TABLE;
        INSCRIPTION_DECLARE_CLASS_NAME_RESOLVER;
    };
}