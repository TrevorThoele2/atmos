#pragma once

#include "VulkanIncludes.h"

namespace Atmos::Render::Vulkan
{
    class Image
    {
    public:
        vk::UniqueImage value;
        vk::UniqueDeviceMemory memory;
        vk::Format format;
        vk::ImageLayout layout;

        Image(
            vk::Format format,
            vk::ImageUsageFlags usage,
            vk::Extent3D extent,
            uint32_t layerCount,
            vk::ImageLayout initialLayout,
            vk::Device device,
            vk::PhysicalDeviceMemoryProperties memoryProperties);

        void TransitionLayout(
            vk::ImageLayout newLayout,
            uint32_t layerOffset,
            uint32_t layerCount,
            vk::Device device,
            vk::CommandPool commandPool,
            vk::Queue graphicsQueue);

        void Copy(
            Image& destination,
            vk::Offset3D sourceOffset,
            uint32_t sourceLayerOffset,
            uint32_t sourceLayerCount,
            vk::Offset3D destinationOffset,
            uint32_t destinationLayerOffset,
            uint32_t destinationLayerCount,
            vk::Extent3D extent,
            vk::Device device,
            vk::CommandPool commandPool,
            vk::Queue queue);
    private:
        static vk::AccessFlags AccessFlagsFor(vk::ImageLayout layout);
        static vk::PipelineStageFlags PipelineStageFlagsFor(vk::ImageLayout);
    };
}