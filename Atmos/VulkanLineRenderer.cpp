#include "VulkanLineRenderer.h"

#include "VulkanUniversalData.h"
#include "VulkanUtilities.h"

namespace Atmos::Render::Vulkan
{
    LineRenderer::LineRenderer(
        std::shared_ptr<vk::Device> device,
        vk::Queue graphicsQueue,
        vk::PhysicalDeviceMemoryProperties memoryProperties,
        vk::RenderPass renderPass,
        uint32_t swapchainImageCount,
        vk::Extent2D swapchainExtent,
        const std::vector<const Asset::Material*>& materials)
        :
        vertexBuffer(vertexStride * sizeof(Vertex), *device, memoryProperties, vk::BufferUsageFlagBits::eVertexBuffer),
        descriptorSetPool(
            {
                DescriptorSetPool::Definition
                {
                    vk::DescriptorType::eUniformBuffer,
                    0,
                    vk::ShaderStageFlagBits::eFragment | vk::ShaderStageFlagBits::eVertex,
                    1
                }
            },
            device),
        mappedConduits(
            device,
            memoryProperties,
            VertexInput(
                sizeof(Vertex),
                {
                    vk::VertexInputAttributeDescription(
                        0, 0, vk::Format::eR32G32Sfloat, offsetof(Vertex, position)),
                    vk::VertexInputAttributeDescription(
                        1, 0, vk::Format::eR32G32B32A32Sfloat, offsetof(Vertex, color))
                }),
            vk::PrimitiveTopology::eLineList,
            renderPass,
            swapchainImageCount,
            swapchainExtent,
            { descriptorSetPool.DescriptorSetLayout() }),
        graphicsQueue(graphicsQueue),
        device(device),
        swapchainImageCount(swapchainImageCount)
    {
        for (auto& material : materials)
            MaterialCreated(*material);
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

        auto layer = layers.Find(lineRender.z);
        if (!layer)
            layer = &layers.Add(lineRender.z, Layer{});
        auto& group = layer->GroupFor(*lineRender.material);
        group.ListFor(lineRender.width).emplace_back(points);
    }

    void LineRenderer::Start(
        vk::CommandBuffer commandBuffer,
        vk::CommandPool commandPool,
        uint32_t currentSwapchainImage,
        UniversalData universalData)
    {
        if (layers.Empty())
            return;

        drawContext = DrawContext{};
        drawContext->currentSwapchainImage = currentSwapchainImage;
        drawContext->universalData = universalData;
        drawContext->currentLayer = layers.begin();
        drawContext->commandBuffer = commandBuffer;
        drawContext->commandPool = commandPool;

        descriptorSetPool.Reserve(swapchainImageCount);

        for (uint32_t i = 0; i < swapchainImageCount; ++i)
            drawContext->setupDescriptorSets.emplace_back(i, descriptorSetPool.Next());
    }

    void LineRenderer::DrawNextLayer()
    {
        auto& layer = drawContext->currentLayer->second;

        Draw(layer);

        ++drawContext->currentLayer;
    }

    void LineRenderer::End()
    {
        drawContext.reset();

        layers.Clear();
        descriptorSetPool.Reset();
    }

    void LineRenderer::MaterialCreated(const Asset::Material& material)
    {
        if (material.Type() != Asset::MaterialType::Line)
            return;

        mappedConduits.Add(material);
    }

    void LineRenderer::MaterialDestroying(const Asset::Material& material)
    {
        if (material.Type() != Asset::MaterialType::Line)
            return;

        mappedConduits.Remove(material);
    }

    bool LineRenderer::IsDone() const
    {
        return !drawContext || drawContext->currentLayer == layers.end();
    }

    Spatial::Point3D::Value LineRenderer::NextLayer() const
    {
        return drawContext->currentLayer->first;
    }

    size_t LineRenderer::LayerCount() const
    {
        return layers.Count();
    }

    LineRenderer::Line::Line(const std::vector<Vertex>& points) : vertices(points)
    {}

    void LineRenderer::Draw(Layer& layer)
    {
        const auto& commandBuffer = drawContext->commandBuffer;

        vk::Buffer vertexBuffers[] = { vertexBuffer.destination.value.get() };
        vk::DeviceSize offsets[] = { 0 };
        commandBuffer.bindVertexBuffers(0, 1, vertexBuffers, offsets);

        for (auto& group : layer.materialGroups)
            WriteToBuffers(group.second, *group.first);
    }

    void LineRenderer::WriteToBuffers(
        const Layer::MaterialGroup& group,
        const Asset::Material& materialAsset)
    {
        auto& conduits = *mappedConduits.For(materialAsset);
        for (auto& conduit : conduits)
            for (auto& value : group.values)
                WriteToBuffers(conduit, value.second, value.first);
    }

    void LineRenderer::WriteToBuffers(
        Conduit& conduit,
        const std::vector<Line>& lines,
        LineWidth width)
    {
        const auto& commandBuffer = drawContext->commandBuffer;
        const auto startVertexCount = drawContext->vertexCount;

        std::vector<Vertex> drawnVertices;
        for (auto& line : lines)
        {
            drawnVertices.insert(drawnVertices.begin(), line.vertices.begin(), line.vertices.end());
            drawContext->vertexCount += line.vertices.size();
        }

        auto currentDescriptorSet = std::find_if(
            drawContext->setupDescriptorSets.begin(),
            drawContext->setupDescriptorSets.end(),
            [this](const SetupDescriptorSet& set)
            {
                return set.swapchainImage == drawContext->currentSwapchainImage;
            })->value;
        conduit.PrepareExecution(
            currentDescriptorSet,
            drawContext->currentSwapchainImage,
            drawContext->universalData);
        conduit.Bind(commandBuffer);
        commandBuffer.bindDescriptorSets(
            vk::PipelineBindPoint::eGraphics,
            mappedConduits.PipelineLayout(),
            0,
            1,
            &currentDescriptorSet,
            0,
            nullptr);

        commandBuffer.setLineWidth(width);

        const auto lineVertexOffset = vk::DeviceSize(startVertexCount) * sizeof Vertex;
        const auto lineVertexSize = drawnVertices.size() * sizeof Vertex;

        vertexBuffer.PushSourceBytes(drawnVertices.data(), lineVertexOffset, lineVertexSize);
        vertexBuffer.CopyFromSourceToDestination(lineVertexOffset, lineVertexSize, drawContext->commandPool, graphicsQueue);
        drawContext->commandBuffer.draw(drawnVertices.size(), 1, startVertexCount, 0);
    }
}