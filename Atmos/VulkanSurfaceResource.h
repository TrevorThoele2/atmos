#pragma once

#include "VulkanBackingSurfaceResource.h"

namespace Atmos::Render::Resource::Vulkan
{
    class Surface final : public Resource::Surface
    {
    public:
        BackingSurface* backing;

        Surface(BackingSurface& backing);
        
        void OnMaterialDestroying(const Arca::Index<Asset::Material>& material) override;

        [[nodiscard]] Spatial::Size2D Size() const override;
    };
}