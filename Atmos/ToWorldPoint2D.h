#pragma once

#include <Arca/Command.h>
#include <Arca/Reliquary.h>

#include "Point2D.h"

namespace Atmos::Spatial
{
    struct ToWorldPoint2D
    {
        Point2D from;

        ToWorldPoint2D(Point2D from);
        Point2D Do(Arca::Reliquary& reliquary) const;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Spatial::ToWorldPoint2D>
    {
        static const ObjectType objectType = ObjectType::Command;
        static TypeName TypeName() { return "Atmos::Spatial::ToWorldPoint2D"; }
        using Result = Atmos::Spatial::Point2D;
        static const bool selfContained = true;
    };
}