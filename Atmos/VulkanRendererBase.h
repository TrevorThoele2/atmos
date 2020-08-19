#pragma once

#include "VulkanIncludes.h"

#include "VulkanUniversalData.h"
#include "MaterialAsset.h"
#include "Point3D.h"

namespace Atmos::Render::Vulkan
{
    class RendererBase
    {
    public:
        virtual ~RendererBase() = 0;

        virtual void Start(
            vk::CommandBuffer commandBuffer,
            vk::CommandPool commandPool,
            uint32_t currentSwapchainImage,
            UniversalData universalData) = 0;
        virtual void DrawNextLayer() = 0;
        virtual void End() = 0;

        virtual void MaterialCreated(const Asset::Material& material) = 0;
        virtual void MaterialDestroying(const Asset::Material& material) = 0;

        [[nodiscard]] virtual bool IsDone() const = 0;
        [[nodiscard]] virtual Spatial::Point3D::Value NextLayer() const = 0;
        [[nodiscard]] virtual uint32_t LayerCount() const = 0;
    };
}