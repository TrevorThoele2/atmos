#pragma once

#include "Point3D.h"
#include "Size2D.h"
#include "Angle2D.h"
#include "Scalers2D.h"
#include "AxisAlignedBox2D.h"
#include "Color.h"
#include "MaterialAsset.h"
#include "SurfaceResource.h"
#include "FontAssetResource.h"

namespace Atmos::Render
{
    struct RenderText
    {
        String string;

        Asset::Resource::Font* fontResource;
        Spatial::AxisAlignedBox2D viewSlice;
        
        const Asset::Material* material;

        Spatial::Point3D position;
        Spatial::Angle2D rotation;
        Spatial::Scalers2D scalers;

        Color color;

        bool bold;
        bool italics;
        float wrapWidth;

        int space;
        
        Resource::Surface* surface;
    };
}