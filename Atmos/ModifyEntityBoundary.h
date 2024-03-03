#pragma once

#include <Arca/Command.h>
#include "GridPoint.h"

namespace Atmos::World
{
    struct ModifyEntityBoundary
    {
        std::vector<Spatial::Grid::Point> add;
        std::vector<Spatial::Grid::Point> remove;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::World::ModifyEntityBoundary>
    {
        static const ObjectType objectType = ObjectType::Command;
        static TypeName TypeName() { return "Atmos::World::ModifyEntityBoundary"; }
    };
}