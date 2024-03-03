
#include "CharacterClassSystem.h"

namespace Atmos
{
    CharacterClassSystem::CharacterClassSystem(ObjectManager& manager) : RegistryObjectSystem(manager)
    {}

    CharacterClassSystem::CharacterClassSystem(const ::Inscription::Table<CharacterClassSystem>& table) : INSCRIPTION_TABLE_GET_BASE(RegistryObjectSystem)
    {}
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DEFINE_TABLE(::Atmos::CharacterClassSystem)
    {
        INSCRIPTION_INSCRIPTER_CREATE_TABLE;

        INSCRIPTION_TABLE_ADD_BASE(::Atmos::ObjectSystem);

        INSCRIPTION_INSCRIPTER_RETURN_TABLE;
    }

    INSCRIPTION_DEFINE_SIMPLE_CLASS_NAME_RESOLVER(::Atmos::CharacterClassSystem, "CharacterClassSystem");
}