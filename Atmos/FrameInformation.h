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
        Time::Point<> startTime;
        Time::Point<> endTime;

        Time::Duration<> lastElapsed = Time::Duration<>{ 0 };
        Time::Duration<> totalElapsed = Time::Duration<>{ 0 };

        FramesPerSecond framesPerSecond = 0;

        struct Profilers
        {
            Time::Stopwatch render;
            Time::Stopwatch frame;
            Time::Stopwatch idle;

            Profilers();
        } profilers;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Frame::Information>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static TypeName TypeName() { return "Atmos::Frame::Information"; }
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