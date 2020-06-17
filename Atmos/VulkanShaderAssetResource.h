#pragma once

#include "ShaderAssetResource.h"
#include "VulkanIncludes.h"

namespace Atmos::Asset::Resource::Vulkan
{
    class Shader final : public Resource::Shader
    {
    public:
        Shader(
            vk::ShaderModule module,
            std::shared_ptr<vk::Device> device);
        ~Shader();

        [[nodiscard]] vk::ShaderModule Module() const;
    private:
        vk::ShaderModule module;
        std::shared_ptr<vk::Device> device;
    };
}