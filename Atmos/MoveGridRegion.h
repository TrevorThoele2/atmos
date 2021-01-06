#pragma once

#include <Arca/Command.h>
#include <Arca/RelicID.h>

#include "GridPoint.h"

#include <unordered_set>

namespace Atmos::Render
{
    struct MoveGridRegion
    {
        Arca::RelicID id = Arca::nullRelicID;

        std::optional<std::unordered_set<Spatial::Grid::Point>> points;
        std::optional<Spatial::Grid::Point::Value> z;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::MoveGridRegion>
    {
        static const ObjectType objectType = ObjectType::Command;
        static TypeName TypeName() { return "Atmos::Render::MoveGridRegion"; }
    };
}