#pragma once

#include <Arca/Command.h>
#include <Arca/Reliquary.h>

#include "Point2D.h"
#include "Point3D.h"

namespace Atmos::Spatial
{
    struct ToWorldPoint3D
    {
        Point2D from;
        Point3D::Value z;

        ToWorldPoint3D(Point2D from, Point3D::Value z);
        Point3D Do(Arca::Reliquary& reliquary) const;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Spatial::ToWorldPoint3D>
    {
        static const ObjectType objectType = ObjectType::Command;
        static TypeName TypeName() { return "Atmos::Spatial::ToWorldPoint3D"; }
        using Result = Atmos::Spatial::Point3D;
        static const bool selfContained = true;
    };
}