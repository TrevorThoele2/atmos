#include "VulkanPipeline.h"

#include "VulkanUniversalData.h"
#include "VulkanShaderAssetResource.h"

namespace Atmos::Render::Vulkan
{
    Pipeline::Pipeline(
        const Asset::Material& material,
        vk::Device device,
        uint32_t swapchainImageCount,
        vk::PhysicalDeviceMemoryProperties memoryProperties,
        const std::vector<vk::DescriptorSetLayout>& descriptorSetLayouts,
        vk::RenderPass renderPass,
        VertexInput vertexInput,
        vk::Extent2D swapchainExtent,
        vk::PrimitiveTopology primitiveTopology,
        std::vector<vk::DynamicState> dynamicStates)
        :
        Pipeline(
            material.VertexShader().Get(),
            material.FragmentShader().Get(),
            device,
            swapchainImageCount,
            memoryProperties,
            descriptorSetLayouts,
            renderPass,
            vertexInput,
            swapchainExtent,
            primitiveTopology,
            dynamicStates)
    {
        this->material = &material;
    }

    Pipeline::Pipeline(
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
        std::vector<vk::DynamicState> dynamicStates)
        :
        extent(swapchainExtent)
    {
        std::vector<vk::PipelineShaderStageCreateInfo> shaderStages;
        if (vertexShader)
            shaderStages.push_back(ShaderStageCreateInfo(*vertexShader, vk::ShaderStageFlagBits::eVertex));
        if (fragmentShader)
            shaderStages.push_back(ShaderStageCreateInfo(*fragmentShader, vk::ShaderStageFlagBits::eFragment));

        const vk::PipelineLayoutCreateInfo layoutCreateInfo({}, descriptorSetLayouts.size(), descriptorSetLayouts.data());
        layout = device.createPipelineLayoutUnique(layoutCreateInfo);

        auto vertexInputCreateInfo = vertexInput.PipelineCreateInfo();

        vk::PipelineInputAssemblyStateCreateInfo inputAssemblyCreateInfo(
            {}, primitiveTopology, VK_FALSE);

        vk::Viewport viewport(
            0.0f,
            0.0f,
            static_cast<float>(swapchainExtent.width),
            static_cast<float>(swapchainExtent.height),
            0.0f,
            1.0f);

        vk::Rect2D scissor({ 0, 0 }, swapchainExtent);

        vk::PipelineViewportStateCreateInfo viewportStateCreateInfo({}, 1, &viewport, 1, &scissor);

        vk::PipelineRasterizationStateCreateInfo rasterizerCreateInfo(
            {},
            VK_FALSE,
            VK_FALSE,
            vk::PolygonMode::eFill,
            vk::CullModeFlagBits::eBack,
            vk::FrontFace::eClockwise,
            VK_FALSE,
            0.0f,
            0.0f,
            0.0f,
            1.0f);

        vk::PipelineMultisampleStateCreateInfo multisamplingCreateInfo(
            {},
            vk::SampleCountFlagBits::e1,
            VK_FALSE,
            1.0f,
            nullptr,
            VK_FALSE,
            VK_FALSE);

        vk::PipelineColorBlendAttachmentState colorBlendAttachment(
            VK_TRUE,
            vk::BlendFactor::eSrcAlpha,
            vk::BlendFactor::eOneMinusSrcAlpha,
            vk::BlendOp::eAdd,
            vk::BlendFactor::eOne,
            vk::BlendFactor::eZero,
            vk::BlendOp::eAdd,
            vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
            vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA);

        vk::PipelineColorBlendStateCreateInfo colorBlendCreateInfo(
            {},
            VK_FALSE,
            vk::LogicOp::eCopy,
            1,
            &colorBlendAttachment,
            {});

        vk::PipelineDynamicStateCreateInfo dynamicCreateInfo(
            {},
            dynamicStates.size(),
            dynamicStates.data());

        vk::GraphicsPipelineCreateInfo pipelineCreateInfo(
            {},
            shaderStages.size(),
            shaderStages.data(),
            &vertexInputCreateInfo,
            &inputAssemblyCreateInfo,
            nullptr,
            &viewportStateCreateInfo,
            &rasterizerCreateInfo,
            &multisamplingCreateInfo,
            nullptr,
            &colorBlendCreateInfo,
            !dynamicStates.empty() ? &dynamicCreateInfo : nullptr,
            layout.get(),
            renderPass,
            0,
            nullptr,
            0);

        value = device.createGraphicsPipelineUnique(nullptr, pipelineCreateInfo);

        uniformBuffers = CreateUniformBuffers(device, memoryProperties, swapchainImageCount);

        uniformBufferDescriptors.reserve(this->uniformBuffers.size());
        for (auto& buffer : this->uniformBuffers)
            uniformBufferDescriptors.emplace_back(buffer.value.get(), 0, sizeof(UniversalData), 0);
    }

    vk::PipelineShaderStageCreateInfo Pipeline::ShaderStageCreateInfo(
        const Asset::Shader& shaderAsset, vk::ShaderStageFlagBits shaderType)
    {
        const auto fileData = shaderAsset.ResourceAs<Asset::Resource::Vulkan::Shader>();
        return vk::PipelineShaderStageCreateInfo(
            {},
            shaderType,
            fileData->Module(),
            shaderAsset.EntryPoint().c_str());
    }

    std::vector<Buffer> Pipeline::CreateUniformBuffers(
        vk::Device device, vk::PhysicalDeviceMemoryProperties memoryProperties, size_t size)
    {
        std::vector<Buffer> returnValue;
        returnValue.reserve(size);
        for (size_t i = 0; i < size; ++i)
            returnValue.emplace_back(
                sizeof(Vulkan::UniversalData),
                vk::BufferUsageFlagBits::eUniformBuffer,
                vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
                device,
                memoryProperties);
        return returnValue;
    }
}
