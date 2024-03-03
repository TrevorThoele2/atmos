#pragma once

#include <Arca/Shard.h>
#include "TimeValue.h"

namespace Atmos::Time
{
    struct StopwatchStatistics
    {
        Nanoseconds average;
        Nanoseconds highest;

        StopwatchStatistics();
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Time::StopwatchStatistics>
    {
        static const ObjectType objectType = ObjectType::Shard;
        static inline const TypeName typeName = "Atmos::Time::StopwatchStatistics";
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Time::StopwatchStatistics, BinaryArchive> final : public
        ArcaNullScribe<Atmos::Time::StopwatchStatistics, BinaryArchive>
    {};
}