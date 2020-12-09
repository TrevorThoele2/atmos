#pragma once

#include "RenderMesh.h"
#include "MaterialAsset.h"

namespace Atmos::Render
{
    struct RegionRender
    {
        Mesh mesh;
        Spatial::Point2D::Value z;

        Arca::RelicID materialID;
        const Asset::Material* material;
    };
}