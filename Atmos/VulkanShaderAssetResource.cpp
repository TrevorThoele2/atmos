#include "VulkanShaderAssetResource.h"

namespace Atmos::Asset::Resource::Vulkan
{
    Shader::Shader(
        vk::ShaderModule module,
        std::shared_ptr<vk::Device> device)
        :
        module(module),
        device(device)
    {}

    Shader::~Shader()
    {
        device->destroy(module);
    }

    vk::ShaderModule Shader::Module() const
    {
        return module;
    }
}