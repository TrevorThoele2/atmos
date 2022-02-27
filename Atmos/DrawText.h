#pragma once

#include "Point2D.h"
#include "Angle2D.h"
#include "Scalers2D.h"
#include "AxisAlignedBox2D.h"
#include "Color.h"
#include "MaterialAsset.h"
#include "SurfaceResource.h"
#include "FontAssetResource.h"

namespace Atmos::Render::Raster
{
    struct DrawText
    {
        String string;
        Asset::Resource::Font* fontResource = nullptr;
        Spatial::AxisAlignedBox2D viewSlice;
        const Asset::Material* material = nullptr;
        Color color;
        bool bold = false;
        bool italics = false;
        float wrapWidth = 0;
        Resource::Surface* surface = nullptr;

        Spatial::Point2D position;
        Spatial::Angle2D rotation = 0;
        Spatial::Scalers2D scalers;
    };
}