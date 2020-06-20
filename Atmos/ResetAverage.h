#pragma once

#include <Arca/Command.h>
#include "Arca/RelicID.h"

namespace Atmos::Time
{
    struct ResetAverage
    {
        Arca::RelicID id;

        explicit ResetAverage(Arca::RelicID id) : id(id)
        {}
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Time::ResetAverage>
    {
        static const ObjectType objectType = ObjectType::Command;
        static inline const TypeName typeName = "Atmos::Time::ResetAverage";
    };
}