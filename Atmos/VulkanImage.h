#pragma once

#include "VulkanIncludes.h"
#include "VulkanCommand.h"
#include "VulkanCombinedImageSamplerDescriptor.h"
#include "VulkanStoredResource.h"

#include "Size2D.h"

namespace Atmos::Render::Vulkan
{
    class ImageData final
    {
    public:
        Spatial::Size2D size;
        vk::ImageView imageView;
        vk::ImageLayout imageLayout;
        vk::Image image;
        CombinedImageSamplerDescriptor descriptor;
        StoredResource* storedResource;
    public:
        ImageData(
            Spatial::Size2D size,
            vk::Image image,
            vk::ImageView imageView,
            vk::ImageLayout imageLayout,
            CombinedImageSamplerDescriptor descriptor,
            StoredResource& storedResource);
    };

    [[nodiscard]] Command TransitionLayout(vk::Image image, vk::ImageLayout from, vk::ImageLayout to, uint32_t layerOffset, uint32_t layerCount);
    [[nodiscard]] Command TransitionLayout(ImageData& imageData, vk::ImageLayout to, uint32_t layerOffset, uint32_t layerCount);
    [[nodiscard]] vk::AccessFlags AccessFlags(vk::ImageLayout layout);
    [[nodiscard]] vk::PipelineStageFlags PipelineStageFlags(vk::ImageLayout);
}