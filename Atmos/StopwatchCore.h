#pragma once

#include <Arca/Shard.h>
#include "TimeValue.h"

namespace Atmos::Time
{
    struct StopwatchCore
    {
        Value start;
        std::function<Value()> currentTime;

        StopwatchCore() = default;
        explicit StopwatchCore(std::function<Value()>&& currentTime);
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Time::StopwatchCore>
    {
        static const ObjectType objectType = ObjectType::Shard;
        static inline const TypeName typeName = "StopwatchCore";
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Time::StopwatchCore, BinaryArchive> final : public
        ArcaNullScribe<Atmos::Time::StopwatchCore, BinaryArchive>
    {};
}