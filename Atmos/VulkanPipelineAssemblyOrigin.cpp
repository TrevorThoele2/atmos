#include "VulkanPipelineAssemblyOrigin.h"

namespace Atmos::Render::Vulkan
{
    PipelineAssemblyOrigin::PipelineAssemblyOrigin(std::uint32_t imageCount) :
        imageCount(imageCount)
    {}

    PipelineAssemblyOrigin& PipelineAssemblyOrigin::UniformBuffer(std::uint32_t binding)
    {
        bindings.emplace_back(
            binding,
            vk::DescriptorType::eUniformBuffer,
            1,
            vk::ShaderStageFlagBits::eVertex);

        poolSizes.emplace_back(vk::DescriptorType::eUniformBuffer, imageCount);

        return *this;
    }

    PipelineAssemblyOrigin& PipelineAssemblyOrigin::CombinedImageSampler(std::uint32_t binding)
    {
        bindings.emplace_back(
            binding,
            vk::DescriptorType::eCombinedImageSampler,
            1,
            vk::ShaderStageFlagBits::eFragment);

        poolSizes.emplace_back(vk::DescriptorType::eCombinedImageSampler, imageCount);

        return *this;
    }

    PipelineAssembly PipelineAssemblyOrigin::Assembly(vk::Device device)
    {
        const vk::DescriptorPoolCreateInfo descriptorPoolCreateInfo({}, imageCount, poolSizes.size(), poolSizes.data());
        auto descriptorPool = device.createDescriptorPoolUnique(descriptorPoolCreateInfo);

        const vk::DescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo({}, bindings.size(), bindings.data());
        auto descriptorSetLayout = device.createDescriptorSetLayoutUnique(descriptorSetLayoutCreateInfo);

        const std::vector<vk::DescriptorSetLayout> layouts(imageCount, *descriptorSetLayout);
        const vk::DescriptorSetAllocateInfo descriptorSetAllocateInfo(*descriptorPool, imageCount, layouts.data());
        auto descriptorSets = device.allocateDescriptorSets(descriptorSetAllocateInfo);

        return PipelineAssembly{
            std::move(descriptorPool),
            std::move(descriptorSetLayout),
            std::move(descriptorSets) };
    }
}
