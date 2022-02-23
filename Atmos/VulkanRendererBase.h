#pragma once

#include "VulkanIncludes.h"
#include "VulkanRaster.h"
#include "VulkanUniversalDataBuffer.h"

#include "AllRenders.h"

namespace Atmos::Render::Vulkan
{
    class RendererBase
    {
    public:
        virtual ~RendererBase() = 0;
        
        [[nodiscard]] virtual std::unique_ptr<Raster> Start(
            const AllRenders& allRenders,
            vk::CommandBuffer commandBuffer,
            const UniversalDataBuffer& universalDataBuffer) = 0;
    };
}