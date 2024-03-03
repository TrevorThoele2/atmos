#pragma once

#include "VulkanIncludes.h"

namespace Atmos::Render::Vulkan
{
    class VertexInput
    {
    public:
        VertexInput(
            size_t stride,
            const std::vector<vk::VertexInputAttributeDescription>& attributes);

        vk::PipelineVertexInputStateCreateInfo PipelineCreateInfo();
    private:
        vk::VertexInputBindingDescription description;
        std::vector<vk::VertexInputAttributeDescription> attributes;
    };
}