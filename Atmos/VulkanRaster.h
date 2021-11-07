#pragma once

#include "Point3D.h"
#include "VulkanObjectLayering.h"
#include "VulkanCommand.h"

namespace Atmos::Render::Vulkan
{
    class Raster
    {
    public:
        struct Pass
        {
            Command writeData;
            Command draw;
        };
    public:
        virtual ~Raster() = 0;

        [[nodiscard]] virtual std::vector<Pass> NextPasses() = 0;

        [[nodiscard]] virtual bool IsDone() const = 0;
        [[nodiscard]] virtual ObjectLayeringKey NextLayer() const = 0;
    };
}