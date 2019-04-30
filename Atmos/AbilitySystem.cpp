
#include "AbilitySystem.h"

namespace Atmos
{
    AbilitySystem::AbilitySystem(ObjectManager& manager) : RegistryObjectSystem(manager)
    {}

    AbilitySystem::AbilitySystem(const ::Inscription::Table<AbilitySystem>& table) : INSCRIPTION_TABLE_GET_BASE(RegistryObjectSystem)
    {}
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DEFINE_TABLE(::Atmos::AbilitySystem)
    {
        INSCRIPTION_INSCRIPTER_CREATE_TABLE;

        INSCRIPTION_TABLE_ADD_BASE(::Atmos::ObjectSystem);

        INSCRIPTION_INSCRIPTER_RETURN_TABLE;
    }

    INSCRIPTION_DEFINE_SIMPLE_CLASS_NAME_RESOLVER(::Atmos::AbilitySystem, "AbilitySystem");
}