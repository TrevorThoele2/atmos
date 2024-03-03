#pragma once

#include "VulkanIncludes.h"
#include "VulkanRaster.h"
#include "VulkanUniversalDataBuffer.h"

#include "MaterialAsset.h"

namespace Atmos::Render::Vulkan
{
    class RendererBase
    {
    public:
        virtual ~RendererBase() = 0;

        [[nodiscard]] virtual std::unique_ptr<Raster> Start(
            vk::CommandBuffer commandBuffer,
            vk::CommandPool commandPool,
            const UniversalDataBuffer& universalDataBuffer) = 0;

        virtual void MaterialCreated(Arca::Index<Asset::Material> material) = 0;
        virtual void MaterialDestroying(Arca::Index<Asset::Material> material) = 0;

        [[nodiscard]] virtual uint32_t RenderCount() const = 0;
    };
}