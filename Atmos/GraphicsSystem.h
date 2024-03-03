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
}

namespace Atmos
{
    template<>
    struct ObjectSystemTraits<Render::GraphicsSystem>
    {
        static const ObjectSystemPriority priority = 0;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Render::GraphicsSystem, BinaryArchive> :
        public ObjectSystemScribe<::Atmos::Render::GraphicsSystem, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}