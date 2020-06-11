#include "VulkanImage.h"

#include "VulkanMemory.h"
#include "VulkanSingleUseCommandBuffer.h"

#include "GraphicsError.h"

namespace Atmos::Render::Vulkan
{
    Image::Image(
        vk::Format format,
        vk::ImageUsageFlags usage,
        vk::Extent3D extent,
        uint32_t layerCount,
        vk::ImageLayout initialLayout,
        vk::Device device,
        vk::PhysicalDeviceMemoryProperties memoryProperties)
    {
        const vk::ImageCreateInfo createInfo(
            {},
            vk::ImageType::e2D,
            format,
            extent,
            1,
            layerCount,
            vk::SampleCountFlagBits::e1,
            vk::ImageTiling::eOptimal,
            usage,
            vk::SharingMode::eExclusive,
            {},
            {},
            initialLayout);
        value = device.createImageUnique(createInfo);

        const auto destinationMemoryRequirements = device.getImageMemoryRequirements(value.get());

        const vk::MemoryAllocateInfo destinationMemoryAllocationInfo(
            destinationMemoryRequirements.size,
            FindSuitableMemoryType(
                destinationMemoryRequirements.memoryTypeBits,
                vk::MemoryPropertyFlagBits::eDeviceLocal,
                memoryProperties));

        memory = device.allocateMemoryUnique(destinationMemoryAllocationInfo);
        device.bindImageMemory(value.get(), memory.get(), 0);

        this->format = format;
        this->layout = initialLayout;
    }

    void Image::TransitionLayout(
        vk::ImageLayout newLayout,
        uint32_t layerOffset,
        uint32_t layerCount,
        vk::Device device,
        vk::CommandPool commandPool,
        vk::Queue graphicsQueue)
    {
        SingleUseCommandBuffer(device, commandPool, graphicsQueue,
            [this, newLayout, layerOffset, layerCount](vk::CommandBuffer commandBuffer)
            {
                const vk::ImageSubresourceRange subresourceRange(
                    vk::ImageAspectFlagBits::eColor,
                    0,
                    1,
                    layerOffset,
                    layerCount);
                const vk::ImageMemoryBarrier barrier(
                    AccessFlagsFor(this->layout),
                    AccessFlagsFor(newLayout),
                    this->layout,
                    newLayout,
                    VK_QUEUE_FAMILY_IGNORED,
                    VK_QUEUE_FAMILY_IGNORED,
                    *this->value,
                    subresourceRange);

                const auto sourceStage = PipelineStageFlagsFor(this->layout);
                const auto destinationStage = PipelineStageFlagsFor(newLayout);

                commandBuffer.pipelineBarrier(sourceStage, destinationStage, {}, nullptr, nullptr, barrier);

                this->layout = newLayout;
            });
    }

    void Image::Copy(
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
        vk::Queue queue)
    {
        SingleUseCommandBuffer(device, commandPool, queue,
            [
                this,
                &destination,
                sourceOffset,
                sourceLayerOffset,
                sourceLayerCount,
                destinationOffset,
                destinationLayerOffset,
                destinationLayerCount,
                extent
            ]
        (vk::CommandBuffer commandBuffer)
            {
                const vk::ImageSubresourceLayers sourceSubresourceLayers(
                    vk::ImageAspectFlagBits::eColor, 0, sourceLayerOffset, sourceLayerCount);
                const vk::ImageSubresourceLayers destinationSubresourceLayers(
                    vk::ImageAspectFlagBits::eColor, 0, destinationLayerOffset, destinationLayerCount);
                const vk::ImageCopy region(
                    sourceSubresourceLayers,
                    sourceOffset,
                    destinationSubresourceLayers,
                    destinationOffset,
                    extent);

                commandBuffer.copyImage(
                    value.get(),
                    layout,
                    destination.value.get(),
                    destination.layout,
                    region);
            });
    }

    vk::AccessFlags Image::AccessFlagsFor(vk::ImageLayout layout)
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

    vk::PipelineStageFlags Image::PipelineStageFlagsFor(vk::ImageLayout layout)
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