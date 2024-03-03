#pragma once

#include <Arca/Curator.h>

namespace Atmos::Render
{
    class CanvasViewCurator final : public Arca::Curator
    {
    public:
        void Work();
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::CanvasViewCurator>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static inline const TypeName typeName = "CanvasViewCurator";
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Render::CanvasViewCurator, BinaryArchive> final
        : public ArcaNullScribe<Atmos::Render::CanvasViewCurator, BinaryArchive>
    {};
}