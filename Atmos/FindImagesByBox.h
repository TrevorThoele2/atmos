#pragma once

#include <Arca/Command.h>

#include "AxisAlignedBox2D.h"
#include "AxisAlignedBox3D.h"
#include <Arca/RelicID.h>

#include "SpatialAlgorithms.h"

namespace Atmos::Render
{
    struct FindImagesByBox
    {
        Spatial::AxisAlignedBox3D box;

        FindImagesByBox(Spatial::AxisAlignedBox3D box) : box(box)
        {}

        FindImagesByBox(Spatial::AxisAlignedBox2D box) :
            box(
                ToPoint3D(box.center, 0),
                Spatial::Size3D{box.size.width, box.size.height, std::numeric_limits<Spatial::Size3D::Value>::max()})
        {}
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::FindImagesByBox>
    {
        static const ObjectType objectType = ObjectType::Command;
        static TypeName TypeName() { return "Atmos::Render::FindImagesByBox"; }
        using Result = std::vector<RelicID>;
    };
}