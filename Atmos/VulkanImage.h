#pragma once

#include "VulkanIncludes.h"
#include "VulkanCommand.h"
#include "VulkanCombinedImageSamplerDescriptor.h"
#include "VulkanStoredResource.h"

#include "Color.h"
#include "Size2D.h"

namespace Atmos::Render::Vulkan
{
    static constexpr auto defaultImageFormat = vk::Format::eR8G8B8A8Srgb;

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

    enum class InitialImageLayout
    {
        Undefined,
        Preinitialized
    };

    [[nodiscard]] vk::UniqueImage CreateImage(
        vk::Device device, uint32_t width, uint32_t height, vk::Format format, vk::ImageUsageFlags usage, InitialImageLayout layout);
    [[nodiscard]] vk::UniqueImageView CreateImageView(
        vk::Image image, vk::Device device, vk::Format imageFormat, uint32_t layerOffset, uint32_t layerCount);
    [[nodiscard]] Command TransitionLayout(
        vk::Image image, vk::ImageLayout from, vk::ImageLayout to, uint32_t layerOffset, uint32_t layerCount);
    [[nodiscard]] Command TransitionLayout(
        ImageData& imageData, vk::ImageLayout to, uint32_t layerOffset, uint32_t layerCount);
    [[nodiscard]] Command Copy(
        vk::Image from, vk::ImageLayout fromLayout, vk::Image to, vk::ImageLayout toLayout, const std::vector<vk::ImageCopy>& regions);
    [[nodiscard]] Command Copy(
        vk::Buffer from, vk::Image to, vk::ImageLayout toLayout, const std::vector<vk::BufferImageCopy>& regions);
    [[nodiscard]] Command ClearImage(
        vk::Image image, vk::ImageLayout imageLayout, const Color& color);
    [[nodiscard]] vk::AccessFlags AccessFlags(
        vk::ImageLayout layout);
    [[nodiscard]] vk::PipelineStageFlags PipelineStageFlags(
        vk::ImageLayout layout);
}