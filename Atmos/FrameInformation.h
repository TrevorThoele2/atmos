#pragma once

#include <Arca/Relic.h>
#include "FramesPerSecond.h"

#include "TimePoint.h"
#include "Stopwatch.h"

namespace Atmos::Frame
{
    class Information final
    {
    public:
        Information();

        Time::Point<> startTime;
        Time::Point<> endTime;

        Time::Duration<> lastElapsed;
        Time::Duration<> totalElapsed;

        FramesPerSecond framesPerSecond = 0;

        Time::Stopwatch stopwatch;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Frame::Information>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static inline const TypeName typeName = "Atmos::Frame::Information";
        static const Locality locality = Locality::Global;
    };
}

namespace Inscription
{
    template<class Archive>
    struct ScribeTraits<Atmos::Frame::Information, Archive> final
    {
        using Category = ArcaNullScribeCategory<Atmos::Frame::Information>;
    };
}