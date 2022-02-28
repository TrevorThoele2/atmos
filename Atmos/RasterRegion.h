#pragma once

#include "RenderMesh.h"
#include "Material.h"

namespace Atmos::Render::Raster
{
    struct Region
    {
        Mesh mesh;
        Material material;
    };
}