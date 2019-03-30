
#include "StatusEffectSystem.h"

namespace Atmos
{
    StatusEffectSystem::StatusEffectSystem(ObjectManager& manager) : RegistryObjectSystem(manager)
    {}

    StatusEffectSystem::StatusEffectSystem(const ::Inscription::Table<StatusEffectSystem>& table) : INSCRIPTION_TABLE_GET_BASE(RegistryObjectSystem)
    {}
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DEFINE_TABLE(::Atmos::StatusEffectSystem)
    {
        INSCRIPTION_INSCRIPTER_CREATE_TABLE;
        INSCRIPTION_INSCRIPTER_RETURN_TABLE;
    }

    INSCRIPTION_DEFINE_SIMPLE_CLASS_NAME_RESOLVER(::Atmos::StatusEffectSystem, "StatusEffectSystem");
}