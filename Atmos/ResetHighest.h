#pragma once

#include <Arca/Command.h>
#include "Arca/RelicID.h"

namespace Atmos::Time
{
    struct ResetHighest
    {
        Arca::RelicID id;

        explicit ResetHighest(Arca::RelicID id) : id(id)
        {}
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Time::ResetHighest>
    {
        static const ObjectType objectType = ObjectType::Command;
        static inline const TypeName typeName = "ResetHighest";
    };
}