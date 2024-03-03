
#include "WorldSystem.h"

namespace Atmos
{
    WorldSystem::WorldSystem(ObjectManager& manager) : UnownedProviderSystem(manager)
    {}

    WorldSystem::WorldSystem(const ::Inscription::Table<WorldSystem>& table) :
        INSCRIPTION_TABLE_GET_BASE(UnownedProviderSystem<WorldManager>)
    {}
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DEFINE_TABLE(::Atmos::WorldSystem)
    {
        INSCRIPTION_INSCRIPTER_CREATE_TABLE;

        INSCRIPTION_TABLE_ADD_BASE(::Atmos::UnownedProviderSystem<::Atmos::WorldManager>);

        INSCRIPTION_INSCRIPTER_RETURN_TABLE;
    }

    INSCRIPTION_DEFINE_SIMPLE_CLASS_NAME_RESOLVER(::Atmos::WorldSystem, "WorldSystem");
}