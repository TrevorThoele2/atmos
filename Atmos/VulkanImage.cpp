#include "VulkanImage.h"

#include "VulkanMemory.h"

#include "GraphicsError.h"

namespace Atmos::Render::Vulkan
{
    vk::ImageLayout ToImageLayout(InitialImageLayout layout)
    {
        switch (layout)
        {
        case InitialImageLayout::Undefined:
            return vk::ImageLayout::eUndefined;
        case InitialImageLayout::Preinitialized:
            return vk::ImageLayout::ePreinitialized;
        }

        throw GraphicsError("Unknown initial image layout.");
    }

    ImageData::ImageData(
        Spatial::Size2D size,
        vk::Image image,
        vk::ImageView imageView,
        vk::ImageLayout imageLayout,
        CombinedImageSamplerDescriptor descriptor,
        StoredResource& storedResource)
        :
        size(size),
        imageView(imageView),
        imageLayout(imageLayout),
        image(image),
        descriptor(descriptor),
        storedResource(&storedResource)
    {}

    vk::UniqueImage CreateImage(
        vk::Device device, uint32_t width, uint32_t height, vk::Format format, vk::ImageUsageFlags usage, InitialImageLayout layout)
    {
        if (width <= 0 || height <= 0)
            const auto wait = 1 + 1;

        return device.createImageUnique(vk::ImageCreateInfo(
            {},
            vk::ImageType::e2D,
            format,
            vk::Extent3D(width, height, 1),
            1,
            1,
            vk::SampleCountFlagBits::e1,
            vk::ImageTiling::eOptimal,
            usage,
            vk::SharingMode::eExclusive,
            {},
            {},
            ToImageLayout(layout)));
    }

    vk::UniqueImageView CreateImageView(
        vk::Image image, vk::Device device, vk::Format imageFormat, uint32_t layerOffset, uint32_t layerCount)
    {
        const vk::ImageViewCreateInfo createInfo(
            {},
            image,
            vk::ImageViewType::e2D,
            imageFormat,
            vk::ComponentMapping(),
            vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, layerOffset, layerCount));

        return device.createImageViewUnique(createInfo);
    }

    Command TransitionLayout(
        vk::Image image, vk::ImageLayout from, vk::ImageLayout to, uint32_t layerOffset, uint32_t layerCount)
    {
        return [image, from, to, layerOffset, layerCount](vk::CommandBuffer commandBuffer)
        {
            const vk::ImageSubresourceRange subresourceRange(
                vk::ImageAspectFlagBits::eColor,
                0,
                1,
                layerOffset,
                layerCount);
            const vk::ImageMemoryBarrier barrier(
                AccessFlags(from),
                AccessFlags(to),
                from,
                to,
                VK_QUEUE_FAMILY_IGNORED,
                VK_QUEUE_FAMILY_IGNORED,
                image,
                subresourceRange);

            const auto sourceStage = PipelineStageFlags(from);
            const auto destinationStage = PipelineStageFlags(to);

            commandBuffer.pipelineBarrier(sourceStage, destinationStage, {}, nullptr, nullptr, barrier);
        };
    }

    Command TransitionLayout(
        ImageData& imageData, vk::ImageLayout to, uint32_t layerOffset, uint32_t layerCount)
    {
        const auto fromLayout = imageData.imageLayout;
        imageData.imageLayout = to;
        return TransitionLayout(imageData.image, fromLayout, to, layerOffset, layerCount);
    }

    Command Copy(
        vk::Image from, vk::ImageLayout fromLayout, vk::Image to, vk::ImageLayout toLayout, const std::vector<vk::ImageCopy>& regions)
    {
        return [from, fromLayout, to, toLayout, regions](vk::CommandBuffer commandBuffer)
        {
            commandBuffer.copyImage(from, fromLayout, to, toLayout, regions);
        };
    }

    Command Copy(
        vk::Buffer from, vk::Image to, vk::ImageLayout toLayout, const std::vector<vk::BufferImageCopy>& regions)
    {
        return [from, to, toLayout, regions](vk::CommandBuffer commandBuffer)
        {
            commandBuffer.copyBufferToImage(from, to, toLayout, regions);
        };
    }

    Command ClearImage(
        vk::Image image, vk::ImageLayout imageLayout, const Color& color)
    {
        return [image, imageLayout, color](vk::CommandBuffer commandBuffer)
        {
            const vk::ImageSubresourceRange subresourceRange{ vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1 };

            const auto useColor = std::array<float, 4>
            {
                static_cast<float>(color.red),
                static_cast<float>(color.green),
                static_cast<float>(color.blue),
                static_cast<float>(color.alpha)
            };
            const vk::ClearColorValue clearColor(useColor);
            commandBuffer.clearColorImage(image, imageLayout, clearColor, subresourceRange);
        };
    }

    vk::AccessFlags AccessFlags(
        vk::ImageLayout layout)
    {
        switch (layout)
        {
        case vk::ImageLayout::eUndefined:
            return {};
        case vk::ImageLayout::eTransferSrcOptimal:
            return vk::AccessFlagBits::eTransferRead;
        case vk::ImageLayout::eTransferDstOptimal:
            return vk::AccessFlagBits::eTransferWrite;
        case vk::ImageLayout::eShaderReadOnlyOptimal:
            return vk::AccessFlagBits::eShaderRead;
        case vk::ImageLayout::eGeneral:
            return vk::AccessFlagBits::eTransferRead
                | vk::AccessFlagBits::eTransferWrite
                | vk::AccessFlagBits::eShaderRead;
        default:
            throw GraphicsError("Unsupported layout transition.");
        }
    }

    vk::PipelineStageFlags PipelineStageFlags(
        vk::ImageLayout layout)
    {
        switch (layout)
        {
        case vk::ImageLayout::eUndefined:
            return vk::PipelineStageFlagBits::eTopOfPipe;
        case vk::ImageLayout::eTransferSrcOptimal:
            return vk::PipelineStageFlagBits::eTransfer;
        case vk::ImageLayout::eTransferDstOptimal:
            return vk::PipelineStageFlagBits::eTransfer;
        case vk::ImageLayout::eShaderReadOnlyOptimal:
            return vk::PipelineStageFlagBits::eFragmentShader;
        case vk::ImageLayout::eGeneral:
            return vk::PipelineStageFlagBits::eTransfer | vk::PipelineStageFlagBits::eFragmentShader;
        default:
            throw GraphicsError("Unsupported layout transition.");
        }
    }
}