#include "VulkanVertexInput.h"

namespace Atmos::Render::Vulkan
{
    VertexInput::VertexInput(
        size_t stride,
        const std::vector<vk::VertexInputAttributeDescription>& attributes)
        :
        description(0, stride, vk::VertexInputRate::eVertex),
        attributes(attributes)
    {}

    vk::PipelineVertexInputStateCreateInfo VertexInput::PipelineCreateInfo()
    {
        return
        {
            {},
            1,
            &description,
            attributes.size(),
            attributes.data()
        };
    }
}