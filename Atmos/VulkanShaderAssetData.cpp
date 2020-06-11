#include "VulkanShaderAssetData.h"

namespace Atmos::Render::Vulkan
{
    ShaderAssetDataImplementation::ShaderAssetDataImplementation(
        vk::ShaderModule module,
        std::shared_ptr<vk::Device> device)
        :
        module(module),
        device(device)
    {}

    ShaderAssetDataImplementation::~ShaderAssetDataImplementation()
    {
        device->destroy(module);
    }

    vk::ShaderModule ShaderAssetDataImplementation::Module() const
    {
        return module;
    }
}