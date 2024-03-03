#pragma once

#include "ImageAsset.h"
#include "Position3D.h"
#include "Size2D.h"
#include "AxisAlignedBox2D.h"
#include "Color.h"
#include "MaterialAsset.h"

namespace Atmos::Render
{
    struct ImageRender
    {
        const Asset::Image* asset;
        int assetIndex;
        AxisAlignedBox2D assetSlice;
        const Asset::Material* material;

        Position3D position;
        Size2D size;

        Color color;
    };
}