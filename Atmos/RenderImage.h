#pragma once

#include "ImageAssetResource.h"
#include "Point3D.h"
#include "Size2D.h"
#include "Angle2D.h"
#include "Scalers2D.h"
#include "AxisAlignedBox2D.h"
#include "Color.h"
#include "MaterialAsset.h"
#include "SurfaceResource.h"

namespace Atmos::Render
{
    struct RenderImage
    {
        Asset::Resource::Image* assetResource;
        Spatial::AxisAlignedBox2D slice;
        
        Arca::Index<Asset::Material> material;

        Spatial::Point3D position;
        Spatial::Angle2D rotation;
        Spatial::Scalers2D scalers;

        Color color;

        int space;

        Resource::Surface* surface;
    };
}