#pragma once

#include <Arca/Curator.h>

namespace Atmos::Render
{
    class CanvasViewCurator final : public Arca::Curator
    {
    protected:
        void InitializeImplementation() override;
        void WorkImplementation(Stage& stage) override;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::CanvasViewCurator>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static const TypeName typeName;
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Render::CanvasViewCurator, BinaryArchive> final
        : public ArcaNullScribe<Atmos::Render::CanvasViewCurator, BinaryArchive>
    {};
}