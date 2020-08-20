#pragma once

#include "Point3D.h"

namespace Atmos::Render::Vulkan
{
    class Raster
    {
    public:
        virtual ~Raster() = 0;

        virtual void DrawNextLayer() = 0;

        [[nodiscard]] virtual bool IsDone() const = 0;
        [[nodiscard]] virtual Spatial::Point3D::Value NextLayer() const = 0;
    };
}