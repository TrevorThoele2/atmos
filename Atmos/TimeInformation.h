#pragma once

#include <Arca/ClosedTypedRelic.h>

#include "TimeValue.h"
#include "Fps.h"
#include "RealStopwatch.h"

namespace Atmos::Time
{
    class Information final : public Arca::ClosedTypedRelic<Information>
    {
    public:
        explicit Information(Init init);

        Value frameStartTime;
        Value frameEndTime;

        Value lastFrameElapsed;
        Value totalElapsed;

        Fps fps = 0;

        using Stopwatch = Arca::Index<RealStopwatch>;
        Stopwatch stopwatch;
    };
}

namespace Arca
{
    template<>
    struct Traits<::Atmos::Time::Information>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static inline const TypeName typeName = "TimeInformation";
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