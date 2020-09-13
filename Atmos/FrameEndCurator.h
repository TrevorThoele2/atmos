#pragma once

#include <Arca/Curator.h>

#include "Stopwatch.h"

#include "DebugProfiler.h"

namespace Atmos::Frame
{
    class EndCurator final : public Arca::Curator
    {
    public:
        explicit EndCurator(Init init);

        void Work();
    private:
        Time::Stopwatch framesPerSecondStopwatch;
    private:
        Debug::Profiler debugIdleProfiler;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Frame::EndCurator>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static inline const TypeName typeName = "Atmos::Frame::EndCurator";
    };
}

namespace Inscription
{
    template<class Archive>
    struct ScribeTraits<Atmos::Frame::EndCurator, Archive> final
    {
        using Category = ArcaNullScribeCategory<Atmos::Frame::EndCurator>;
    };
}