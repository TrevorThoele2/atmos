#pragma once

#include <Arca/Command.h>

#include "AxisAlignedBox2D.h"
#include "AxisAlignedBox3D.h"
#include <Arca/RelicID.h>

#include "SpatialAlgorithms.h"

namespace Atmos::Render
{
    struct FindGridRegionsByBox
    {
        Spatial::AxisAlignedBox3D box;

        FindGridRegionsByBox(Spatial::AxisAlignedBox3D box) : box(box)
        {}

        FindGridRegionsByBox(Spatial::AxisAlignedBox2D box) :
            box(
                ToPoint3D(box.center, 0),
                Spatial::Size3D{ box.size.width, box.size.height, std::numeric_limits<Spatial::Size3D::Value>::max() })
        {}
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::FindGridRegionsByBox>
    {
        static const ObjectType objectType = ObjectType::Command;
        static TypeName TypeName() { return "Atmos::Render::FindGridRegionsByBox"; }
        using Result = std::vector<RelicID>;
    };
}