#pragma once

#include "RegistryObjectSystem.h"

#include "StatusEffect.h"

#include "Serialization.h"

namespace Atmos
{
    class StatusEffectSystem : public RegistryObjectSystem<nStatusEffect>
    {
    public:
        StatusEffectSystem(ObjectManager& manager);
        StatusEffectSystem(const ::Inscription::Table<StatusEffectSystem>& table);
    };
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DECLARE(::Atmos::StatusEffectSystem)
    {
    public:
        INSCRIPTION_INSCRIPTER_DECLARE_TABLE;
        INSCRIPTION_DECLARE_CLASS_NAME_RESOLVER;
    };
}