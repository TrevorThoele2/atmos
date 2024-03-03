#pragma once

#include "MaterialAsset.h"
#include "Position3D.h"
#include "Size2D.h"
#include "AxisAlignedBox2D.h"
#include "Color.h"
#include "ShaderAsset.h"

namespace Atmos::Render
{
    struct MaterialRender
    {
        Asset::MaterialAsset* material;

        Position3D position;
        Size2D size;

        Color color;

        AxisAlignedBox2D materialSlice;

        Asset::ShaderAsset* patchShader;
    };
}