#pragma once

#include "Position2D.h"
#include "MaterialAsset.h"

namespace Atmos::Render
{
    struct RegionRender
    {
        std::vector<Position2D> points;
        Position2D::Value z;
        const Asset::Material* material;
    };
}