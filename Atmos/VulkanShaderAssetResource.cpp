#include "VulkanShaderAssetResource.h"

namespace Atmos::Asset::Resource::Vulkan
{
    Shader::Shader(vk::UniqueShaderModule&& module) :
        module(std::move(module))
    {}

    vk::ShaderModule Shader::Module() const
    {
        return *module;
    }
}