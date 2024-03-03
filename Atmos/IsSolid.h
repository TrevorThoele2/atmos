#pragma once

#include <Arca/Command.h>
#include "GridPoint.h"

namespace Atmos::World
{
    struct IsSolid
    {
        Spatial::Grid::Point point;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::World::IsSolid>
    {
        static const ObjectType objectType = ObjectType::Command;
        static TypeName TypeName() { return "Atmos::World::IsSolid"; }
        using Result = bool;
    };
}