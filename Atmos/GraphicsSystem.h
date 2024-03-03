#pragma once

#include "UniqueProviderSystem.h"
#include "GraphicsManager.h"

namespace Atmos
{
    class GraphicsSystem : public UniqueProviderSystem<GraphicsManager>
    {
    public:
        GraphicsSystem(ObjectManager& manager);
        GraphicsSystem(const ::Inscription::Table<GraphicsSystem>& table);
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
        INSCRIPTION_INSCRIPTER_DECLARE_TABLE;
        INSCRIPTION_DECLARE_CLASS_NAME_RESOLVER;
    };
}