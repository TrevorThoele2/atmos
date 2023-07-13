#pragma once

#include "VulkanBackingSurfaceResource.h"

namespace Atmos::Render::Resource::Vulkan
{
    class Surface final : public Resource::Surface
    {
    public:
        BackingSurface* backing;

        explicit Surface(BackingSurface& backing);
        
        [[nodiscard]] Spatial::Size2D Size() const override;
    };
}