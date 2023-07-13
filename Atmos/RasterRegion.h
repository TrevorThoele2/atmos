#pragma once

#include "RenderMesh.h"
#include "MaterialAsset.h"

namespace Atmos::Render::Raster
{
    struct Region
    {
        Arca::Index<Asset::Material> material;
        Mesh mesh;
    };
}