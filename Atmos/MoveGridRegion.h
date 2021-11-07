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
        static constexpr ObjectType objectType = ObjectType::Command;
        static const inline TypeName typeName = "Atmos::Render::MoveGridRegion";
    };
}