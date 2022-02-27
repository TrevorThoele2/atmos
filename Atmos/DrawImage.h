#pragma once

#include "Shaders.h"
#include "SurfaceResource.h"
#include "ImageAssetResource.h"
#include "Point2D.h"
#include "Size2D.h"
#include "Angle2D.h"
#include "Scalers2D.h"
#include "AxisAlignedBox2D.h"
#include "Color.h"

namespace Atmos::Render::Raster
{
    struct DrawImage
    {
        Shaders shaders;
        const Resource::Surface* surface = nullptr;
        Asset::Resource::Image* assetResource = nullptr;
        Spatial::AxisAlignedBox2D assetSlice;
        Spatial::AxisAlignedBox2D viewSlice;
        Color color;

        Spatial::Point2D position;
        Spatial::Size2D size;
        Spatial::Angle2D rotation = 0;
        Spatial::Scalers2D scalers;
    };
}