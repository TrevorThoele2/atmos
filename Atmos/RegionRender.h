#pragma once

#include "RenderMesh.h"
#include "MaterialAsset.h"

namespace Atmos::Render
{
    struct RegionRender
    {
        Mesh mesh;
        Position2D::Value z;
        const Asset::Material* material;
    };
}