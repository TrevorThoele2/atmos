
#include "SpellSystem.h"

namespace Atmos
{
    SpellSystem::SpellSystem(ObjectManager& manager) : RegistryObjectSystem(manager)
    {}

    SpellSystem::SpellSystem(const ::Inscription::Table<SpellSystem>& table) : INSCRIPTION_TABLE_GET_BASE(RegistryObjectSystem)
    {}
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DEFINE_TABLE(::Atmos::SpellSystem)
    {
        INSCRIPTION_INSCRIPTER_CREATE_TABLE;

        INSCRIPTION_TABLE_ADD_BASE(::Atmos::ObjectSystem);

        INSCRIPTION_INSCRIPTER_RETURN_TABLE;
    }

    INSCRIPTION_DEFINE_SIMPLE_CLASS_NAME_RESOLVER(::Atmos::SpellSystem, "SpellSystem");
}