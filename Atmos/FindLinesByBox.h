#pragma once

#include <Arca/Command.h>

#include "AxisAlignedBox2D.h"
#include "AxisAlignedBox3D.h"
#include <Arca/RelicID.h>

#include "SpatialAlgorithms.h"

namespace Atmos::Render
{
    struct FindLinesByBox
    {
        Spatial::AxisAlignedBox3D box;

        FindLinesByBox(Spatial::AxisAlignedBox3D box) : box(box)
        {}

        FindLinesByBox(Spatial::AxisAlignedBox2D box) :
            box(
                ToPoint3D(box.center, 0),
                Spatial::Size3D{ box.size.width, box.size.height, std::numeric_limits<Spatial::Size3D::Value>::max() })
        {}
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::FindLinesByBox>
    {
        static constexpr ObjectType objectType = ObjectType::Command;
        static const inline TypeName typeName = "Atmos::Render::FindLinesByBox";
        using Result = std::vector<RelicID>;
    };
}