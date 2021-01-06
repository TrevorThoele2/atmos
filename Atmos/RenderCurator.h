#pragma once

#include <Arca/Curator.h>

#include "MainSurface.h"
#include "Work.h"
#include "ChangeColor.h"

#include "DebugProfiler.h"

namespace Atmos::Render
{
    class Curator final : public Arca::Curator
    {
    public:
        explicit Curator(Init init);

        void Handle(const Work& command);
        void Handle(const ChangeColor& command);
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
        static TypeName TypeName() { return "Atmos::Render::Curator"; }
        using HandledCommands = HandledCommands<
            Atmos::Work,
            Atmos::Render::ChangeColor>;
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