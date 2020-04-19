#pragma once

#include "VulkanIncludes.h"
#include "VulkanPipelineAssembly.h"

namespace Atmos::Render::Vulkan
{
    class PipelineAssemblyOrigin
    {
    public:
        PipelineAssemblyOrigin(std::uint32_t imageCount);

        PipelineAssemblyOrigin& UniformBuffer(std::uint32_t binding);
        PipelineAssemblyOrigin& CombinedImageSampler(std::uint32_t binding);
    public:
        PipelineAssembly Assembly(vk::Device device);
    private:
        std::uint32_t imageCount;
        std::vector<vk::DescriptorSetLayoutBinding> bindings;
        std::vector<vk::DescriptorPoolSize> poolSizes;
    };
}