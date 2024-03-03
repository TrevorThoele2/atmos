
#include "GraphicsSystem.h"

#include "NullGraphics.h"

namespace Atmos
{
    GraphicsSystem::GraphicsSystem(ObjectManager& manager) :
        UniqueProviderSystem(manager, UniqueProviderSystem::ValuePtr(new NullGraphicsManager(manager)))
    {}

    INSCRIPTION_BINARY_TABLE_CONSTRUCTOR_DEFINE(GraphicsSystem) :
        INSCRIPTION_TABLE_GET_BASE(UniqueProviderSystem<GraphicsManager>)
    {}
}

namespace Inscription
{
    INSCRIPTION_BINARY_INSCRIPTER_DEFINE_TABLE(::Atmos::GraphicsSystem)
    {
        INSCRIPTION_BINARY_INSCRIPTER_CREATE_TABLE;

        INSCRIPTION_TABLE_ADD_BASE(::Atmos::UniqueProviderSystem<::Atmos::GraphicsManager>);

        INSCRIPTION_INSCRIPTER_RETURN_TABLE;
    }

    INSCRIPTION_BINARY_DEFINE_SIMPLE_CLASS_NAME_RESOLVER(::Atmos::GraphicsSystem, "GraphicsSystem");
}