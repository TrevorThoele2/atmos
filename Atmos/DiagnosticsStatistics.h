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
    template<>
    class Scribe<Atmos::Diagnostics::Statistics::Profile> final
    {
    public:
        using ObjectT = Atmos::Diagnostics::Statistics::Profile;
    public:
        template<class Format>
        void Scriven(ObjectT& object, Format& format)
        {
            format("time", object.time);
            format("average", object.average);
            format("highest", object.highest);
        }
    };

    template<class Format>
    struct ScribeTraits<Atmos::Diagnostics::Statistics::Profile, Format> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Diagnostics::Statistics::Profile>;
    };

    template<class Format>
    struct ScribeTraits<Atmos::Diagnostics::Statistics, Format> final
    {
        using Category = ArcaNullScribeCategory<Atmos::Diagnostics::Statistics>;
    };
}