#pragma once

#include "RenderMesh.h"
#include "MaterialAsset.h"
#include "SurfaceResource.h"

namespace Atmos::Render
{
    struct RenderRegion
    {
        Mesh mesh;
        Spatial::Point2D::Value z;

        Arca::Index<Asset::Material> material;

        int space;

        Resource::Surface* surface;
    };
}