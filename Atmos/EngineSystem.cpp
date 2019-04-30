
#include "EngineSystem.h"

namespace Atmos
{
    EngineSystem::EngineSystem(ObjectManager& manager) : UnownedProviderSystem(manager)
    {}

    EngineSystem::EngineSystem(const ::Inscription::Table<EngineSystem>& table) : INSCRIPTION_TABLE_GET_BASE(UnownedProviderSystem)
    {}
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DEFINE_TABLE(::Atmos::EngineSystem)
    {
        INSCRIPTION_INSCRIPTER_CREATE_TABLE;

        INSCRIPTION_TABLE_ADD_BASE(::Atmos::UnownedProviderSystem<::Atmos::Engine>);

        INSCRIPTION_INSCRIPTER_RETURN_TABLE;
    }

    INSCRIPTION_DEFINE_SIMPLE_CLASS_NAME_RESOLVER(::Atmos::EngineSystem, "EngineSystem");
}