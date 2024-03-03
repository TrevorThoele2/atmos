#pragma once

#include "VulkanIncludes.h"
#include "VulkanRaster.h"
#include "VulkanUniversalDataBuffer.h"

namespace Atmos::Render::Vulkan
{
    class RendererBase
    {
    public:
        virtual ~RendererBase() = 0;
        
        [[nodiscard]] virtual std::unique_ptr<Raster> Start(
            vk::CommandBuffer commandBuffer,
            const UniversalDataBuffer& universalDataBuffer) = 0;

        [[nodiscard]] virtual uint32_t RenderCount() const = 0;
    };
}