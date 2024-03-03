#pragma once

#include "VulkanIncludes.h"
#include "VulkanBuffer.h"
#include "MaterialAsset.h"
#include "VulkanUniformBufferDescriptor.h"
#include "VulkanVertexInput.h"

namespace Atmos::Render::Vulkan
{
    struct Pipeline
    {
        vk::UniquePipeline value;
        vk::UniquePipelineLayout layout;

        std::vector<Buffer> uniformBuffers;
        std::vector<UniformBufferDescriptor> uniformBufferDescriptors;

        vk::Extent2D extent;

        const Asset::Material* material = nullptr;

        Pipeline() = default;
        Pipeline(
            const Asset::Material& material,
            vk::Device device,
            uint32_t swapchainImageCount,
            vk::PhysicalDeviceMemoryProperties memoryProperties,
            const std::vector<vk::DescriptorSetLayout>& descriptorSetLayouts,
            vk::RenderPass renderPass,
            VertexInput vertexInput,
            vk::Extent2D swapchainExtent,
            vk::PrimitiveTopology primitiveTopology,
            std::vector<vk::DynamicState> dynamicStates);
        Pipeline(
            const Asset::Shader* vertexShader,
            const Asset::Shader* fragmentShader,
            vk::Device device,
            uint32_t swapchainImageCount,
            vk::PhysicalDeviceMemoryProperties memoryProperties,
            const std::vector<vk::DescriptorSetLayout>& descriptorSetLayouts,
            vk::RenderPass renderPass,
            VertexInput vertexInput,
            vk::Extent2D swapchainExtent,
            vk::PrimitiveTopology primitiveTopology,
            std::vector<vk::DynamicState> dynamicStates);

        Pipeline(Pipeline&& arg) noexcept = default;
        Pipeline& operator=(Pipeline&& arg) = default;
    private:
        [[nodiscard]] static vk::PipelineShaderStageCreateInfo ShaderStageCreateInfo(
            const Asset::Shader& shaderAsset, vk::ShaderStageFlagBits shaderType);

        [[nodiscard]] static std::vector<Buffer> CreateUniformBuffers(
            vk::Device device, vk::PhysicalDeviceMemoryProperties memoryProperties, size_t size);
    };
}
