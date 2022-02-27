#pragma once

#include "RenderMesh.h"
#include "Shaders.h"
#include "SurfaceResource.h"

namespace Atmos::Render::Raster
{
    struct DrawRegion
    {
        Shaders shaders;
        const Resource::Surface* surface = nullptr;
        Mesh mesh;
    };
}