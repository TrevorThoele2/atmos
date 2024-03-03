#pragma once

#include "ShaderAssetData.h"
#include "VulkanIncludes.h"

namespace Atmos::Render::Vulkan
{
    class ShaderAssetDataImplementation final : public Asset::ShaderData
    {
    public:
        ShaderAssetDataImplementation(
            vk::ShaderModule module,
            std::shared_ptr<vk::Device> device);
        ~ShaderAssetDataImplementation();

        [[nodiscard]] vk::ShaderModule Module() const;
    private:
        vk::ShaderModule module;
        std::shared_ptr<vk::Device> device;
    };
}