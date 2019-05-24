#pragma once

#include "UniqueProviderSystem.h"
#include "GraphicsManager.h"

namespace Atmos
{
    class GraphicsSystem : public UniqueProviderSystem<GraphicsManager>
    {
    public:
        GraphicsSystem(ObjectManager& manager);
        INSCRIPTION_BINARY_TABLE_CONSTRUCTOR_DECLARE(GraphicsSystem);
    };

    template<>
    struct ObjectSystemTraits<GraphicsSystem>
    {
        static const ObjectSystemPriority priority = 0;
    };
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DECLARE(::Atmos::GraphicsSystem)
    {
    public:
        INSCRIPTION_BINARY_INSCRIPTER_DECLARE_TABLE;
        INSCRIPTION_BINARY_DECLARE_CLASS_NAME_RESOLVER;
    };
}