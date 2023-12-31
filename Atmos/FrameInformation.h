#pragma once

#include "ArcaRelicIncludes.h"

#include "FramesPerSecond.h"
#include "Stopwatch.h"

namespace Atmos::Frame
{
    class Information final
    {
    public:
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
        static constexpr ObjectType objectType = ObjectType::Relic;
        static const inline TypeName typeName = "Atmos::Frame::Information";
        static constexpr Locality locality = Locality::Global;
    };
}

namespace Inscription
{
    template<class Format>
    struct ScribeTraits<Atmos::Frame::Information, Format> final
    {
        using Category = ArcaNullScribeCategory<Atmos::Frame::Information>;
    };
}