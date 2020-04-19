#pragma once

#include "VulkanIncludes.h"

namespace Atmos::Render::Vulkan
{
    struct PipelineAssembly
    {
        vk::UniqueDescriptorPool descriptorPool;
        vk::UniqueDescriptorSetLayout descriptorSetLayout;
        std::vector<vk::DescriptorSet> descriptorSets;
    };
}