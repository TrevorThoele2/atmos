#pragma once

#include "RenderMesh.h"
#include "MaterialAsset.h"

namespace Atmos::Render
{
    struct RegionRender
    {
        Mesh mesh;
        Spatial::Point2D::Value z;

        Arca::Index<Asset::Material> material;

        int space;
    };
}