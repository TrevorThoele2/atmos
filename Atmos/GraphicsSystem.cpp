
#include "GraphicsSystem.h"

namespace Atmos
{
    GraphicsSystem::GraphicsSystem(ObjectManager& manager) : UniqueProviderSystem(manager)
    {}

    GraphicsSystem::GraphicsSystem(const ::Inscription::Table<GraphicsSystem>& table) :
        INSCRIPTION_TABLE_GET_BASE(UniqueProviderSystem<GraphicsManager>)
    {}
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DEFINE_TABLE(::Atmos::GraphicsSystem)
    {
        INSCRIPTION_INSCRIPTER_CREATE_TABLE;

        INSCRIPTION_TABLE_ADD_BASE(::Atmos::UniqueProviderSystem<::Atmos::GraphicsManager>);

        INSCRIPTION_INSCRIPTER_RETURN_TABLE;
    }

    INSCRIPTION_DEFINE_SIMPLE_CLASS_NAME_RESOLVER(::Atmos::GraphicsSystem, "GraphicsSystem");
}