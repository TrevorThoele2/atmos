#pragma once

#include <Arca/Curator.h>

#include "RealStopwatch.h"
#include "TimeInformation.h"
#include "TimeSettings.h"

#include "DebugStatistics.h"

namespace Atmos
{
    class FrameEndCurator final : public Arca::Curator
    {
    public:
        explicit FrameEndCurator(Init init);

        void Work();
    private:
        using FpsStopwatch = Arca::RelicIndex<Time::RealStopwatch>;
        FpsStopwatch fpsTimer;

        Arca::GlobalIndex<Time::Information> timeInformation;
        Arca::GlobalIndex<Time::Settings> timeSettings;
    private:
        Arca::GlobalIndex<Debug::Statistics> debugStatistics;
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