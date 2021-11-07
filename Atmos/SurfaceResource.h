#pragma once

#include "MaterialAsset.h"

#include "Size2D.h"
#include "Color.h"

namespace Atmos::Render::Resource
{
    class Surface
    {
    public:
        virtual ~Surface() = 0;
        
        virtual void OnMaterialDestroying(const Arca::Index<Asset::Material>& material) = 0;

        [[nodiscard]] virtual Spatial::Size2D Size() const = 0;
    };
}