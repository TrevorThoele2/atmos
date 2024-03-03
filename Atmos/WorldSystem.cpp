
#include "WorldSystem.h"

namespace Atmos
{
    WorldSystem::WorldSystem(ObjectManager& manager) : UnownedProviderSystem(manager)
    {}

    INSCRIPTION_BINARY_TABLE_CONSTRUCTOR_DEFINE(WorldSystem) :
        INSCRIPTION_TABLE_GET_BASE(UnownedProviderSystem<WorldManager>)
    {}
}

namespace Inscription
{
    INSCRIPTION_BINARY_INSCRIPTER_DEFINE_TABLE(::Atmos::WorldSystem)
    {
        INSCRIPTION_BINARY_INSCRIPTER_CREATE_TABLE;

        INSCRIPTION_TABLE_ADD_BASE(::Atmos::UnownedProviderSystem<::Atmos::WorldManager>);

        INSCRIPTION_INSCRIPTER_RETURN_TABLE;
    }

    INSCRIPTION_BINARY_DEFINE_SIMPLE_CLASS_NAME_RESOLVER(::Atmos::WorldSystem, "WorldSystem");
}