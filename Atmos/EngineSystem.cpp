
#include "EngineSystem.h"

namespace Atmos
{
    EngineSystem::EngineSystem(ObjectManager& manager) : UnownedProviderSystem(manager)
    {}

    INSCRIPTION_BINARY_TABLE_CONSTRUCTOR_DEFINE(EngineSystem) : INSCRIPTION_TABLE_GET_BASE(UnownedProviderSystem)
    {}
}

namespace Inscription
{
    INSCRIPTION_BINARY_INSCRIPTER_DEFINE_TABLE(::Atmos::EngineSystem)
    {
        INSCRIPTION_BINARY_INSCRIPTER_CREATE_TABLE;

        INSCRIPTION_TABLE_ADD_BASE(::Atmos::UnownedProviderSystem<::Atmos::Engine>);

        INSCRIPTION_INSCRIPTER_RETURN_TABLE;
    }

    INSCRIPTION_BINARY_DEFINE_SIMPLE_CLASS_NAME_RESOLVER(::Atmos::EngineSystem, "EngineSystem");
}