#pragma once

#include <Arca/Curator.h>

#include "MainSurface.h"

#include "DebugProfiler.h"

namespace Atmos::Render
{
    class Curator final : public Arca::Curator
    {
    public:
        explicit Curator(Init init);

        void Work();
    private:
        Arca::Index<MainSurface> mainSurface;
    private:
        Debug::Profiler debugRenderProfiler;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::Curator>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static inline TypeName typeName = "RenderCurator";
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Render::Curator, BinaryArchive> final :
        public ArcaNullScribe<::Atmos::Render::Curator, BinaryArchive>
    {};
}