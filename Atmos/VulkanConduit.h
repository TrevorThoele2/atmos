#pragma once

#include "VulkanIncludes.h"
#include "VulkanConduitExecutionContext.h"
#include "VulkanVertexInput.h"
#include "VulkanUniversalData.h"
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
            uint32_t swapchainImageCount,
            vk::PhysicalDeviceMemoryProperties memoryProperties,
            vk::RenderPass renderPass,
            VertexInput vertexInput,
            vk::Extent2D swapchainExtent,
            vk::PrimitiveTopology primitiveTopology,
            std::vector<vk::DynamicState> dynamicStates);

        Conduit(Conduit&& arg) noexcept = default;
        Conduit& operator=(Conduit&& arg) = default;

        void PrepareExecution(
            vk::DescriptorSet descriptorSet, std::int32_t currentSwapchainImage, UniversalData universalData);

        void Bind(const vk::CommandBuffer& commandBuffer);
    private:
        vk::UniquePipeline pipeline;

        using ExecutionContext = ConduitExecutionContext;
        std::vector<ExecutionContext> executionContexts;
    private:
        vk::Device device;
    private:
        [[nodiscard]] static vk::PipelineShaderStageCreateInfo ShaderStageCreateInfo(
            const Asset::Shader& shaderAsset, vk::ShaderStageFlagBits shaderType);
    };
}
