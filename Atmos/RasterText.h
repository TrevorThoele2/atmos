#pragma once

#include "Point2D.h"
#include "Angle2D.h"
#include "Scalers2D.h"
#include "AxisAlignedBox2D.h"
#include "Color.h"
#include "Material.h"
#include "FontAssetResource.h"

namespace Atmos::Render::Raster
{
    struct Text
    {
        String string;
        Asset::Resource::Font* fontResource = nullptr;
        Spatial::AxisAlignedBox2D viewSlice;
        Material material;
        Color color;
        bool bold = false;
        bool italics = false;
        float wrapWidth = 0;

        Spatial::Point2D position;
        Spatial::Angle2D rotation = 0;
        Spatial::Scalers2D scalers;
    };
}