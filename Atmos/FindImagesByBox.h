#pragma once

#include <Arca/Command.h>

#include "Space.h"
#include "AxisAlignedBox2D.h"
#include "AxisAlignedBox3D.h"
#include <Arca/RelicID.h>

#include "SpatialAlgorithms.h"

namespace Atmos::Render
{
    struct FindImagesByBox
    {
        Spatial::AxisAlignedBox3D box;
        Spatial::Space space;

        FindImagesByBox(Spatial::AxisAlignedBox3D box, Spatial::Space space) : box(box), space(space)
        {}

        FindImagesByBox(Spatial::AxisAlignedBox2D box, Spatial::Space space) :
            box(
                ToPoint3D(box.center, 0),
                Spatial::Size3D{box.size.width, box.size.height, std::numeric_limits<Spatial::Size3D::Value>::max()}),
            space(space)
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