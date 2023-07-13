#pragma once

#include "ImageAsset.h"
#include "Point2D.h"
#include "Size2D.h"
#include "Angle2D.h"
#include "Scalers2D.h"
#include "AxisAlignedBox2D.h"
#include "Color.h"
#include "MaterialAsset.h"

namespace Atmos::Render::Raster
{
    struct Image
    {
        Arca::Index<Asset::Material> material;
        Arca::Index<Asset::Image> asset;
        Spatial::AxisAlignedBox2D assetSlice;
        Spatial::AxisAlignedBox2D viewSlice;
        Color color;

        Spatial::Point2D position;
        Spatial::Size2D size;
        Spatial::Angle2D rotation = 0;
        Spatial::Scalers2D scalers;
    };
}