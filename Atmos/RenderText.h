#pragma once

#include "Point3D.h"
#include "Size2D.h"
#include "Angle2D.h"
#include "AxisAlignedBox2D.h"
#include "Color.h"
#include "MaterialAsset.h"
#include "TextResource.h"
#include "SurfaceResource.h"

namespace Atmos::Render
{
    struct RenderText
    {
        Resource::Text* resource;
        Spatial::AxisAlignedBox2D slice;
        
        Arca::Index<Asset::Material> material;

        Spatial::Point3D position;
        Spatial::Size2D size;
        Spatial::Angle2D rotation;

        Color color;

        int space;
        
        Resource::Surface* surface;
    };
}