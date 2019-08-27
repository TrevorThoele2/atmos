#pragma once

#include <Arca/ClosedTypedRelicAutomation.h>

#include "TimeValue.h"
#include "Fps.h"
#include "ExtendedStopwatchAdapter.h"
#include "RealStopwatch.h"

namespace Atmos::Time
{
    class Information final : public Arca::ClosedTypedRelicAutomation<Information>
    {
    public:
        Information() = default;

        Value frameStartTime;
        Value frameEndTime;

        Value lastFrameElapsed;
        Value totalElapsed;

        Fps fps = 0;

        using Stopwatch = ExtendedStopwatchAdapter<RealStopwatch>;
        Stopwatch stopwatch;
    };
}

namespace Arca
{
    template<>
    struct Traits<::Atmos::Time::Information>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static const TypeName typeName;
        static const Locality locality = Locality::Global;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Time::Information, BinaryArchive> final :
        public ArcaNullScribe<::Atmos::Time::Information, BinaryArchive>
    {};
}