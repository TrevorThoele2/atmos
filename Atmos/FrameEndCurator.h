#pragma once

#include <Arca/Curator.h>

#include "RealStopwatch.h"
#include "TimeInformation.h"
#include "TimeSettings.h"

#include "DebugProfiler.h"

namespace Atmos
{
    class FrameEndCurator final : public Arca::Curator
    {
    public:
        explicit FrameEndCurator(Init init);

        void Work();
    private:
        using FpsStopwatch = Arca::Index<Time::RealStopwatch>;
        FpsStopwatch fpsTimer;

        Arca::Index<Time::Information> timeInformation;
        Arca::Index<Time::Settings> timeSettings;
    private:
        Debug::Profiler debugIdleProfiler;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::FrameEndCurator>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static inline const TypeName typeName = "FrameEndCurator";
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::FrameEndCurator, BinaryArchive> final
        : public ArcaNullScribe<Atmos::FrameEndCurator, BinaryArchive>
    {};
}