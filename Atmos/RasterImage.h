#pragma once

#include "ImageAssetResource.h"
#include "Point2D.h"
#include "Size2D.h"
#include "Angle2D.h"
#include "Scalers2D.h"
#include "AxisAlignedBox2D.h"
#include "Color.h"
#include "Material.h"

namespace Atmos::Render::Raster
{
    struct Image
    {
        Asset::Resource::Image* assetResource = nullptr;
        Spatial::AxisAlignedBox2D assetSlice;
        Spatial::AxisAlignedBox2D viewSlice;
        Material material;
        Color color;

        Spatial::Point2D position;
        Spatial::Size2D size;
        Spatial::Angle2D rotation = 0;
        Spatial::Scalers2D scalers;
    };
}