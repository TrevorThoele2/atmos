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
        static inline TypeName typeName = "Atmos::Render::Curator";
    };
}

namespace Inscription
{
    template<class Archive>
    struct ScribeTraits<Atmos::Render::Curator, Archive> final
    {
        using Category = ArcaNullScribeCategory<Atmos::Render::Curator>;
    };
}