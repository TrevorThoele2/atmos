#pragma once

#include "RenderMesh.h"
#include "MaterialAsset.h"
#include "SurfaceResource.h"

namespace Atmos::Render::Raster
{
    struct Region
    {
        Mesh mesh;
        Resource::Surface* surface = nullptr;
        const Asset::Material* material = nullptr;
    };
}