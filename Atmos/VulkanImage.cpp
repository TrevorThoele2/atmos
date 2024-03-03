#include "VulkanImage.h"

#include "VulkanMemory.h"

#include "GraphicsError.h"

namespace Atmos::Render::Vulkan
{
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

    Command TransitionLayout(vk::Image image, vk::ImageLayout from, vk::ImageLayout to, uint32_t layerOffset, uint32_t layerCount)
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

    Command TransitionLayout(ImageData& imageData, vk::ImageLayout to, uint32_t layerOffset, uint32_t layerCount)
    {
        const auto fromLayout = imageData.imageLayout;
        imageData.imageLayout = to;
        return TransitionLayout(imageData.image, fromLayout, to, layerOffset, layerCount);
    }

    vk::AccessFlags AccessFlags(vk::ImageLayout layout)
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
        default:
            throw GraphicsError("Unsupported layout transition.");
        }
    }

    vk::PipelineStageFlags PipelineStageFlags(vk::ImageLayout layout)
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
        default:
            throw GraphicsError("Unsupported layout transition.");
        }
    }
}