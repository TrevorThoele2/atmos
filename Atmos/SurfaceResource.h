#pragma once

#include "MaterialAsset.h"

#include "Size2D.h"

namespace Atmos::Render::Resource
{
    class Surface
    {
    public:
        virtual ~Surface() = 0;
        
        [[nodiscard]] virtual Spatial::Size2D Size() const = 0;
    };
}