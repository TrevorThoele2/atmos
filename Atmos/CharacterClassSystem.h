#pragma once

#include "RegistryObjectSystem.h"

#include "CharacterClass.h"

#include "Serialization.h"

namespace Atmos
{
    class CharacterClassSystem : public RegistryObjectSystem<nCharacterClass>
    {
    public:
        CharacterClassSystem(ObjectManager& manager);
        CharacterClassSystem(const ::Inscription::Table<CharacterClassSystem>& table);
    };
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DECLARE(::Atmos::CharacterClassSystem)
    {
    public:
        INSCRIPTION_INSCRIPTER_DECLARE_TABLE;
        INSCRIPTION_DECLARE_CLASS_NAME_RESOLVER;
    };
}