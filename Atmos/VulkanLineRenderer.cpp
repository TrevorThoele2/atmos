#include "VulkanLineRenderer.h"

#include "VulkanUniversalData.h"
#include "VulkanUtilities.h"

namespace Atmos::Render::Vulkan
{
    LineRenderer::LineRenderer(
        std::shared_ptr<vk::Device> device,
        uint32_t graphicsQueueIndex,
        vk::Queue graphicsQueue,
        vk::PhysicalDeviceMemoryProperties memoryProperties)
        :
        vertexBuffer(vertexStride * sizeof(Vertex), *device, memoryProperties, vk::BufferUsageFlagBits::eVertexBuffer),
        core(
            device,
            memoryProperties,
            {
                DescriptorSetGroup::Definition
                {
                    vk::DescriptorType::eUniformBuffer,
                    0,
                    vk::ShaderStageFlagBits::eFragment | vk::ShaderStageFlagBits::eVertex,
                    1
                }
            },
            VertexInput(
                sizeof(Vertex),
                {
                    vk::VertexInputAttributeDescription(
                        0, 0, vk::Format::eR32G32Sfloat, offsetof(Vertex, position)),
                    vk::VertexInputAttributeDescription(
                        1, 0, vk::Format::eR32G32B32A32Sfloat, offsetof(Vertex, color))
                }),
            vk::PrimitiveTopology::eLineList,
            Asset::MaterialType::Line,
            [this](Context& context, DrawContext& drawContext, uint32_t currentImage, UniversalData universalData)
            {
                Draw(context, drawContext, currentImage, universalData);
            }),
        graphicsQueue(graphicsQueue),
        commandBuffers(*device, graphicsQueueIndex),
        device(device)
    {}

    void LineRenderer::Initialize(uint32_t swapchainImageCount, vk::RenderPass renderPass, vk::Extent2D extent)
    {
        core.Initialize(swapchainImageCount, renderPass, extent);
    }

    void LineRenderer::StageRender(const LineRender& lineRender)
    {
        if (lineRender.points.empty())
            return;

        std::vector<Vertex> points;
        for (auto& point : lineRender.points)
        {
            const auto color = AtmosToVulkanColor(lineRender.color);
            points.push_back(Vertex{
                color,
                { point.x, point.y } });
        }

        auto context = core.ContextFor(lineRender.z);
        if (!context)
            context = &core.AddContext(lineRender.z, Context{});
        auto& group = context->GroupFor(*lineRender.material);
        group.ListFor(lineRender.width).emplace_back(points);
    }

    void LineRenderer::Start(const std::vector<const Asset::Material*>& materials, vk::CommandBuffer commandBuffer)
    {
        core.Start(materials, commandBuffer);
    }

    void LineRenderer::DrawNextLayer(uint32_t currentImage, UniversalData universalData)
    {
        core.DrawNextLayer(currentImage, universalData);
    }

    void LineRenderer::End()
    {
        core.End();
    }

    bool LineRenderer::IsDone() const
    {
        return core.IsDone();
    }

    Position3D::Value LineRenderer::NextLayer() const
    {
        return core.NextLayer();
    }

    size_t LineRenderer::LayerCount() const
    {
        return core.LayerCount();
    }

    LineRenderer::Line::Line(const std::vector<Vertex>& points) : vertices(points)
    {}

    auto LineRenderer::Context::Group::ListFor(LineWidth width) -> std::vector<Line>&
    {
        auto found = lines.find(width);
        if (found == lines.end())
            return lines.emplace(width, std::vector<Line>{}).first->second;

        return found->second;
    }

    auto LineRenderer::Context::GroupFor(const Asset::Material& material) -> Group&
    {
        auto found = groups.find(&material);
        if (found == groups.end())
            return groups.emplace(&material, Group{}).first->second;

        return found->second;
    }

    void LineRenderer::Draw(
        Context& context,
        DrawContext& drawContext,
        uint32_t currentImage,
        UniversalData universalData)
    {
        const auto& commandBuffer = drawContext.commandBuffer;

        vk::Buffer vertexBuffers[] = { vertexBuffer.destination.value.get() };
        vk::DeviceSize offsets[] = { 0 };
        commandBuffer.bindVertexBuffers(0, 1, vertexBuffers, offsets);

        for (auto& group : context.groups)
        {
            WriteToBuffers(
                group.second,
                *group.first,
                drawContext,
                currentImage,
                universalData);
        }
    }

    void LineRenderer::WriteToBuffers(
        const Context::Group& group,
        const Asset::Material& materialAsset,
        DrawContext& drawContext,
        uint32_t currentImage,
        UniversalData universalData)
    {
        auto& pipelines = *core.PipelinesFor(materialAsset);
        for (auto& pipeline : pipelines)
        {
            pipeline.uniformBuffers[currentImage].PushBytes(universalData, 0);

            for (auto& object : group.lines)
                WriteToBuffers(
                    object.second,
                    object.first,
                    pipeline,
                    drawContext,
                    currentImage);
        }
    }

    void LineRenderer::WriteToBuffers(
        const std::vector<Line>& lines,
        LineWidth width,
        Pipeline& pipeline,
        DrawContext& drawContext,
        uint32_t currentImage)
    {
        const auto& commandBuffer = drawContext.commandBuffer;
        const auto startVertexCount = drawContext.addition.vertexCount;

        std::vector<Vertex> drawnVertices;
        for (auto& line : lines)
        {
            drawnVertices.insert(drawnVertices.begin(), line.vertices.begin(), line.vertices.end());
            drawContext.addition.vertexCount += line.vertices.size();
        }

        commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline.value.get());

        auto currentDescriptorSet = core.KeyedSetFor(currentImage)->descriptorSet;
        commandBuffer.bindDescriptorSets(
            vk::PipelineBindPoint::eGraphics,
            core.PipelineLayout(),
            0,
            1,
            &currentDescriptorSet,
            0,
            nullptr);

        commandBuffer.setLineWidth(width);

        const auto lineVertexOffset = vk::DeviceSize(startVertexCount) * sizeof Vertex;
        const auto lineVertexSize = drawnVertices.size() * sizeof Vertex;

        vertexBuffer.PushSourceBytes(drawnVertices.data(), lineVertexOffset, lineVertexSize);
        vertexBuffer.CopyFromSourceToDestination(lineVertexOffset, lineVertexSize, commandBuffers.Pool(), graphicsQueue);
        drawContext.commandBuffer.draw(drawnVertices.size(), 1, startVertexCount, 0);
    }
}