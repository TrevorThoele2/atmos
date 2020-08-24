#include "VulkanConduit.h"

#include "VulkanInvalidConduit.h"
#include "VulkanUniversalData.h"
#include "VulkanShaderAssetResource.h"

namespace Atmos::Render::Vulkan
{
    Conduit::Conduit(
        const Asset::Shader* vertexShader,
        const Asset::Shader* fragmentShader,
        vk::Device device,
        vk::PipelineLayout layout,
        vk::RenderPass renderPass,
        VertexInput vertexInput,
        vk::Extent2D swapchainExtent,
        vk::PrimitiveTopology primitiveTopology,
        std::vector<vk::DynamicState> dynamicStates)
        :
        device(device)
    {
        std::vector<vk::PipelineShaderStageCreateInfo> shaderStages;
        if (vertexShader)
            shaderStages.push_back(ShaderStageCreateInfo(*vertexShader, vk::ShaderStageFlagBits::eVertex));
        if (fragmentShader)
            shaderStages.push_back(ShaderStageCreateInfo(*fragmentShader, vk::ShaderStageFlagBits::eFragment));

        if (shaderStages.empty())
            throw InvalidConduit();

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
            layout,
            renderPass,
            0,
            nullptr,
            0);

        pipeline = device.createGraphicsPipelineUnique(nullptr, pipelineCreateInfo);
    }

    void Conduit::Bind(const vk::CommandBuffer& commandBuffer)
    {
        commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline.get());
    }

    vk::PipelineShaderStageCreateInfo Conduit::ShaderStageCreateInfo(
        const Asset::Shader& shaderAsset, vk::ShaderStageFlagBits shaderType)
    {
        const auto fileData = shaderAsset.ResourceAs<Asset::Resource::Vulkan::Shader>();
        return vk::PipelineShaderStageCreateInfo(
            {},
            shaderType,
            fileData->Module(),
            "main");
    }
}
