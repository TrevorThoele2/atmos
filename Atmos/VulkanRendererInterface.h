#pragma once

#include "VulkanIncludes.h"

#include "VulkanUniversalData.h"
#include "MaterialAsset.h"
#include "Position3D.h"

namespace Atmos::Render::Vulkan
{
    class RendererInterface
    {
    public:
        virtual ~RendererInterface() = 0;

        virtual void Initialize(uint32_t swapchainImageCount, vk::RenderPass renderPass, vk::Extent2D extent) = 0;

        virtual void Start(const std::vector<const Asset::Material*>& materials, vk::CommandBuffer commandBuffer) = 0;
        virtual void DrawNextLayer(uint32_t currentImage, UniversalData universalData) = 0;
        virtual void End() = 0;

        [[nodiscard]] virtual bool IsDone() const = 0;
        [[nodiscard]] virtual Position3D::Value NextLayer() const = 0;
        [[nodiscard]] virtual uint32_t LayerCount() const = 0;
    };
}