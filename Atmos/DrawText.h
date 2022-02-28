#pragma once

#include "Point2D.h"
#include "Angle2D.h"
#include "Scalers2D.h"
#include "AxisAlignedBox2D.h"
#include "Color.h"
#include "SurfaceResource.h"
#include "FontAssetResource.h"

#undef DrawText;

namespace Atmos::Render::Raster
{
    struct DrawText
    {
        Shaders shaders;
        Resource::Surface* surface = nullptr;
        String string;
        Asset::Resource::Font* fontResource = nullptr;
        Spatial::AxisAlignedBox2D viewSlice;
        Color color;
        bool bold = false;
        bool italics = false;
        float wrapWidth = 0;

        Spatial::Point2D position;
        Spatial::Angle2D rotation = 0;
        Spatial::Scalers2D scalers;
    };
}