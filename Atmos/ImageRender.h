#pragma once

#include "ImageAsset.h"
#include "Point3D.h"
#include "Size2D.h"
#include "Angle2D.h"
#include "AxisAlignedBox2D.h"
#include "Color.h"
#include "MaterialAsset.h"

namespace Atmos::Render
{
    struct ImageRender
    {
        const Asset::Image* asset;
        int assetIndex;
        Spatial::AxisAlignedBox2D assetSlice;

        Arca::RelicID materialID;
        const Asset::Material* material;

        Spatial::Point3D position;
        Spatial::Size2D size;
        Spatial::Angle2D rotation;

        Color color;
    };
}