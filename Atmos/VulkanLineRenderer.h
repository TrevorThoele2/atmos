#pragma once

#include "VulkanRendererInterface.h"
#include "VulkanRendererCore.h"
#include "VulkanStagedBuffer.h"
#include "VulkanCommandBufferGroup.h"

#include "LineRender.h"
#include "LineWidth.h"
#include "Position3D.h"

#include <glm/glm.hpp>

namespace Atmos::Render::Vulkan
{
    class LineRenderer final : public RendererInterface
    {
    public:
        LineRenderer(
            std::shared_ptr<vk::Device> device,
            uint32_t graphicsQueueIndex,
            vk::Queue graphicsQueue,
            vk::PhysicalDeviceMemoryProperties memoryProperties);

        void Initialize(uint32_t swapchainImageCount, vk::RenderPass renderPass, vk::Extent2D extent) override;

        void StageRender(const LineRender& lineRender);

        void Start(const std::vector<const Asset::Material*>& materials, vk::CommandBuffer commandBuffer) override;
        void DrawNextLayer(uint32_t currentImage, UniversalData universalData) override;
        void End() override;
        [[nodiscard]] bool IsDone() const override;

        [[nodiscard]] Position3D::Value NextLayer() const override;
        [[nodiscard]] size_t LayerCount() const override;
    private:
        struct Vertex
        {
            alignas(16) glm::vec4 color;
            alignas(8) glm::vec2 position;
        };

        struct Line
        {
            using Vertices = std::vector<Vertex>;
            Vertices vertices;
            Line(const std::vector<Vertex>& points);
        };

        static const int stride = 5000;

        StagedBuffer vertexBuffer;
        static const int vertexStride = stride;

        struct Context
        {
            struct Group
            {
                std::map<LineWidth, std::vector<Line>> lines;
                std::vector<Line>& ListFor(LineWidth width);
            };

            std::map<const Asset::Material*, Group> groups;
            Group& GroupFor(const Asset::Material& material);
        };
    private:
        struct DrawContextAddition
        {
            std::uint32_t vertexCount = 0;
        };

        using Core = RendererCore<void, Context, DrawContextAddition>;
        Core core;

        using DrawContext = Core::DrawContext;

        void Draw(
            Context& context,
            DrawContext& drawContext,
            uint32_t currentImage,
            UniversalData universalData);
        void WriteToBuffers(
            const Context::Group& group,
            const Asset::Material& materialAsset,
            DrawContext& drawContext,
            uint32_t currentImage,
            UniversalData universalData);
        void WriteToBuffers(
            const std::vector<Line>& lines,
            LineWidth width,
            Pipeline& pipeline,
            DrawContext& drawContext,
            uint32_t currentImage);
    private:
        vk::Queue graphicsQueue;
        CommandBufferGroup commandBuffers;

        std::shared_ptr<vk::Device> device;
    };
}