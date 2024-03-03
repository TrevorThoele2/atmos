#pragma once

#include <Arca/Command.h>
#include <Arca/RelicID.h>

#include "Angle.h"

namespace Atmos
{
    struct RotateBounds
    {
        Arca::RelicID id = 0;
        Angle to;

        RotateBounds(Arca::RelicID id, const Angle& to) : id(id), to(to)
        {}
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::RotateBounds>
    {
        static const ObjectType objectType = ObjectType::Command;
        static inline const TypeName typeName = "Atmos::RotateBounds";
    };
}