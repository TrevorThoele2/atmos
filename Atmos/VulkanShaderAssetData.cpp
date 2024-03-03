#include "VulkanShaderAssetData.h"

namespace Atmos::Render::Vulkan
{
    ShaderAssetDataImplementation::ShaderAssetDataImplementation(
        vk::ShaderModule module,
        const String& entryPoint,
        std::shared_ptr<vk::Device> device)
        :
        ShaderAssetData(entryPoint),
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