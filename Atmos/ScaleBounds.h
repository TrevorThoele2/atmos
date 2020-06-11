#pragma once

#include <Arca/Command.h>
#include <Arca/RelicID.h>

#include "Scalers2D.h"

namespace Atmos
{
    struct ScaleBounds
    {
        Arca::RelicID id = 0;
        Scalers2D toScalers;

        ScaleBounds(Arca::RelicID id, const Scalers2D& toScalers) : id(id), toScalers(toScalers)
        {}
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::ScaleBounds>
    {
        static const ObjectType objectType = ObjectType::Command;
        static inline const TypeName typeName = "ScaleBounds";
    };
}