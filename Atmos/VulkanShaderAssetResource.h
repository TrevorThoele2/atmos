#pragma once

#include "ShaderAssetResource.h"
#include "VulkanIncludes.h"

namespace Atmos::Asset::Resource::Vulkan
{
    class Shader final : public Resource::Shader
    {
    public:
        Shader(vk::UniqueShaderModule&& module);

        [[nodiscard]] vk::ShaderModule Module() const;
    private:
        vk::UniqueShaderModule module;
    };
}