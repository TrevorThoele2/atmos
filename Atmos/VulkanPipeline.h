#pragma once

#include "VulkanIncludes.h"
#include "VulkanBuffer.h"
#include "MaterialAsset.h"
#include "VulkanUniformBufferDescriptor.h"

namespace Atmos::Render::Vulkan
{
    struct Pipeline
    {
        vk::UniquePipeline value;
        vk::UniquePipelineLayout layout;

        std::vector<Buffer> uniformBuffers;
        std::vector<UniformBufferDescriptor> uniformBufferDescriptors;

        vk::Extent2D extent;

        const Asset::MaterialAsset* material = nullptr;

        Pipeline() = default;
        Pipeline(
            vk::UniquePipeline&& value,
            vk::UniquePipelineLayout&& layout,
            std::vector<Buffer>&& uniformBuffers,
            vk::Extent2D extent);

        Pipeline(Pipeline&& arg) noexcept = default;
        Pipeline& operator=(Pipeline&& arg) = default;
    };
}
