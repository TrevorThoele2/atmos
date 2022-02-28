#pragma once

#include "VulkanIncludes.h"
#include "VulkanVertexInput.h"
#include "VulkanCommand.h"
#include "ShaderAsset.h"

namespace Atmos::Render::Vulkan
{
    class Conduit
    {
    public:
        Conduit() = default;
        Conduit(
            const Asset::Shader* vertexShader,
            const Asset::Shader* fragmentShader,
            vk::Device device,
            vk::PipelineLayout layout,
            vk::RenderPass renderPass,
            VertexInput vertexInput,
            vk::Extent2D swapchainExtent,
            vk::PrimitiveTopology primitiveTopology,
            std::vector<vk::DynamicState> dynamicStates);

        Conduit(Conduit&& arg) noexcept = default;
        Conduit& operator=(Conduit&& arg) = default;

        [[nodiscard]] Command Bind();
    private:
        vk::UniquePipeline pipeline;
    private:
        vk::Device device;
    private:
        [[nodiscard]] static vk::PipelineShaderStageCreateInfo CreateInfo(
            const Asset::Shader& shaderAsset, vk::ShaderStageFlagBits shaderType);
    };
}
