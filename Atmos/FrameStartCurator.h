#pragma once

#include <Arca/Curator.h>

#include "Work.h"
#include "DebugProfiler.h"

namespace Atmos::Frame
{
    class StartCurator final : public Arca::Curator
    {
    public:
        explicit StartCurator(Init init);

        void Handle(const Work& command);
    private:
        Debug::Profiler debugIdleProfiler;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Frame::StartCurator>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static TypeName TypeName() { return "Atmos::Frame::StartCurator"; }
        using HandledCommands = HandledCommands<Atmos::Work>;
    };
}

namespace Inscription
{
    template<class Archive>
    struct ScribeTraits<Atmos::Frame::StartCurator, Archive> final
    {
        using Category = ArcaNullScribeCategory<Atmos::Frame::StartCurator>;
    };
}