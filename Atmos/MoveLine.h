#pragma once

#include <Arca/Command.h>
#include <Arca/RelicID.h>

#include "Point2D.h"

namespace Atmos::Render
{
    struct MoveLine
    {
        Arca::RelicID id = Arca::nullRelicID;

        std::optional<std::vector<Spatial::Point2D>> points;
        std::optional<Spatial::Point2D::Value> z;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::MoveLine>
    {
        static constexpr ObjectType objectType = ObjectType::Command;
        static const inline TypeName typeName = "Atmos::Render::MoveLine";
    };
}