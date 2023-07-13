#pragma once

#include "RenderMesh.h"
#include "Shaders.h"
#include "SurfaceResource.h"

namespace Atmos::Render::Raster
{
    struct DrawRegion
    {
        const Resource::Surface* surface = nullptr;

        Shaders shaders;
        Mesh mesh;
    };
}