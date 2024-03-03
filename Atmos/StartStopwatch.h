#pragma once

#include <Arca/Command.h>
#include "TimeValue.h"
#include <Arca/RelicID.h>

namespace Atmos::Time
{
    struct StartStopwatch
    {
        Arca::RelicID id;

        explicit StartStopwatch(Arca::RelicID id) : id(id)
        {}
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Time::StartStopwatch>
    {
        static const ObjectType objectType = ObjectType::Command;
        static inline const TypeName typeName = "Atmos::Time::StartStopwatch";
        using Result = Atmos::Time::Value<>;
    };
}