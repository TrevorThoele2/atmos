#pragma once

#include "ArcaRelicIncludes.h"
#include "Stopwatch.h"
#include "FramesPerSecond.h"

namespace Atmos::Diagnostics
{
    struct Statistics
    {
        struct Profile
        {
            double time = 0.0;
            double average = 0.0;
            double highest = 0.0;

            bool operator==(const Profile& arg) const;
            bool operator!=(const Profile& arg) const;

            void NewTime(Time::Stopwatch& stopwatch);
        };

        size_t relicCount = 0;
        size_t shardCount = 0;
        Arca::RelicID nextRelicID = 0;

        Profile script;
        Profile render;
        Profile frame;
        Profile idle;
        Profile misc;

        Frame::FramesPerSecond framesPerSecond = 0;

        bool operator==(const Statistics& arg) const;
        bool operator!=(const Statistics& arg) const;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Diagnostics::Statistics>
    {
        static constexpr ObjectType objectType = ObjectType::Relic;
        static const inline TypeName typeName = "Atmos::Diagnostics::Statistics";
        static constexpr Locality locality = Locality::Global;
    };
}

namespace Inscription
{
    template<class Archive>
    struct ScribeTraits<Atmos::Diagnostics::Statistics, Archive> final
    {
        using Category = ArcaNullScribeCategory<Atmos::Diagnostics::Statistics>;
    };
}