#pragma once

#include <Arca/Command.h>
#include <Arca/RelicID.h>

#include "Position2D.h"

namespace Atmos::Render
{
    struct MoveLine
    {
        Arca::RelicID id;

        std::optional<Position2D> from;
        std::optional<Position2D> to;
        std::optional<Position2D::Value> z;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::MoveLine>
    {
        static const ObjectType objectType = ObjectType::Command;
        static inline const TypeName typeName = "MoveLine";
    };
}