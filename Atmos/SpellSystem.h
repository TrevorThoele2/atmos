#pragma once

#include "RegistryObjectSystem.h"

#include "Spell.h"

#include "Serialization.h"

namespace Atmos
{
    class SpellSystem : public RegistryObjectSystem<nSpell>
    {
    public:
        SpellSystem(ObjectManager& manager);
        SpellSystem(const ::Inscription::Table<SpellSystem>& table);
    };
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DECLARE(::Atmos::SpellSystem)
    {
    public:
        INSCRIPTION_INSCRIPTER_DECLARE_TABLE;
        INSCRIPTION_DECLARE_CLASS_NAME_RESOLVER;
    };
}