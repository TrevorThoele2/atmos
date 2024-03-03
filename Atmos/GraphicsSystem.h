#pragma once

#include "UniqueProviderSystem.h"
#include "GraphicsManager.h"

namespace Atmos::Render
{
    class GraphicsSystem : public UniqueProviderSystem<GraphicsManager>
    {
    public:
        GraphicsSystem(ObjectManager& manager);
    };

    template<>
    struct ObjectSystemTraits<GraphicsSystem>
    {
        static const ObjectSystemPriority priority = 0;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::GraphicsSystem, BinaryArchive> :
        public ObjectSystemScribe<::Atmos::GraphicsSystem, BinaryArchive>
    {
    public:
        static void Scriven(ObjectT& object, ArchiveT& archive);
    };
}