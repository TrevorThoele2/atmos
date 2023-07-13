#pragma once

#include "Point2D.h"
#include "Angle2D.h"
#include "Scalers2D.h"
#include "AxisAlignedBox2D.h"
#include "Color.h"
#include "MaterialAsset.h"
#include "FontAsset.h"

namespace Atmos::Render::Raster
{
    struct Text
    {
        Arca::Index<Asset::Material> material;
        String string;
        Arca::Index<Asset::Font> font;
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