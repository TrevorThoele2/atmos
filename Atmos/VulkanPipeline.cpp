#include "VulkanPipeline.h"

#include "VulkanUniversalData.h"

namespace Atmos::Render::Vulkan
{
    Pipeline::Pipeline(
        vk::UniquePipeline&& value,
        vk::UniquePipelineLayout&& layout,
        std::vector<Buffer>&& uniformBuffers,
        vk::Extent2D extent)
        :
        value(std::move(value)),
        layout(std::move(layout)),
        uniformBuffers(std::move(uniformBuffers)),
        extent(extent)
    {
        uniformBufferDescriptors.reserve(this->uniformBuffers.size());
        for (auto& buffer : this->uniformBuffers)
            uniformBufferDescriptors.emplace_back(buffer.value.get(), 0, sizeof(UniversalData), 0);
    }
}
