#pragma once

#include <Arca/Command.h>
#include "TimeValue.h"
#include <Arca/RelicID.h>

namespace Atmos::Time
{
    struct CalculateStopwatch
    {
        Arca::RelicID id;

        explicit CalculateStopwatch(Arca::RelicID id) : id(id)
        {}
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Time::CalculateStopwatch>
    {
        static const ObjectType objectType = ObjectType::Command;
        static inline const TypeName typeName = "CalculateStopwatch";
        using Result = Atmos::Time::Value;
    };
}