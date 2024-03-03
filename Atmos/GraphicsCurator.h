#pragma once

#include <Arca/Curator.h>

#include "GraphicsManager.h"

namespace Atmos::Render
{
    class GraphicsCurator final : public Arca::Curator
    {
    protected:
        void InitializeImplementation() override;
    private:
        Arca::ComputedPtr<GraphicsManager*> manager;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::GraphicsCurator>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static inline const TypeName typeName = "GraphicsCurator";
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Render::GraphicsCurator, BinaryArchive> final
        : public ArcaNullScribe<Atmos::Render::GraphicsCurator, BinaryArchive>
    {};
}