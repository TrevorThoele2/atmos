#pragma once

#include <Arca/Curator.h>

#include "StartStopwatch.h"
#include "CalculateStopwatch.h"
#include "ResetAverage.h"
#include "ResetHighest.h"

namespace Atmos::Time
{
    class StopwatchCurator final : public Arca::Curator
    {
    public:
        explicit StopwatchCurator(Init init);

        Value<> Handle(const StartStopwatch& command);
        Duration<> Handle(const CalculateStopwatch& command);
        void Handle(const ResetAverage& command);
        void Handle(const ResetHighest& command);
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Time::StopwatchCurator>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static inline const TypeName typeName = "Atmos::Time::StopwatchCurator";
        using HandledCommands = HandledCommands<Atmos::Time::StartStopwatch, Atmos::Time::CalculateStopwatch>;
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Time::StopwatchCurator, BinaryArchive> final
        : public ArcaNullScribe<Atmos::Time::StopwatchCurator, BinaryArchive>
    {};
}