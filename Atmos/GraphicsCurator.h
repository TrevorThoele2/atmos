#pragma once

#include <Arca/Curator.h>

#include "GraphicsManagerProvider.h"

namespace Atmos::Render
{
    class GraphicsCurator final : public Arca::Curator
    {
    protected:
        void InitializeImplementation() override;
    private:
        GraphicsManagerProvider* manager = nullptr;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::GraphicsCurator>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static const TypeName typeName;
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Render::GraphicsCurator, BinaryArchive> final
        : public ArcaNullScribe<Atmos::Render::GraphicsCurator, BinaryArchive>
    {};
}