#pragma once

#include "VulkanIncludes.h"

namespace Atmos::Render::Vulkan
{
    class CommandBufferGroup
    {
    public:
        CommandBufferGroup(vk::Device device, uint32_t queueFamily);

        [[nodiscard]] vk::CommandBuffer operator[](uint32_t index) const;

        void Reserve(uint32_t count);
        void DoneWith(vk::CommandBuffer commandBuffer);

        [[nodiscard]] vk::CommandBuffer At(uint32_t index) const;
        [[nodiscard]] vk::CommandBuffer Next();

        [[nodiscard]] size_t Size() const;

        [[nodiscard]] vk::CommandPool Pool() const;
    private:
        vk::UniqueCommandPool pool;
        std::vector<vk::CommandBuffer> availableBuffers;
        std::vector<vk::UniqueCommandBuffer> allBuffers;
    private:
        vk::Device device;
    };
}