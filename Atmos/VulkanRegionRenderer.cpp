#include "VulkanRegionRenderer.h"

#include "VulkanUniversalData.h"

namespace Atmos::Render::Vulkan
{
    RegionRenderer::RegionRenderer(
        std::shared_ptr<vk::Device> device,
        vk::Queue graphicsQueue,
        vk::PhysicalDeviceMemoryProperties memoryProperties,
        vk::RenderPass renderPass,
        uint32_t swapchainImageCount,
        vk::Extent2D swapchainExtent,
        const std::vector<const Asset::Material*>& materials)
        :
        vertexBuffer(vertexStride * sizeof(Vertex), *device, memoryProperties, vk::BufferUsageFlagBits::eVertexBuffer),
        indexBuffer(indexStride * sizeof(Index), *device, memoryProperties, vk::BufferUsageFlagBits::eIndexBuffer),
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
                        0, 0, vk::Format::eR32G32Sfloat, offsetof(Vertex, position))
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

    void RegionRenderer::StageRender(const RegionRender& regionRender)
    {
        if (regionRender.mesh.vertices.empty() || regionRender.mesh.indices.empty())
            return;

        std::vector<Vertex> vertices;
        for (auto& point : regionRender.mesh.vertices)
            vertices.push_back(Vertex{ { point.x, point.y } });

        auto context = layers.Find(regionRender.z);
        if (!context)
            context = &layers.Add(regionRender.z, Layer{});
        auto& group = context->GroupFor(*regionRender.material);
        group.values.emplace_back(vertices, regionRender.mesh.indices);
    }

    void RegionRenderer::Start(
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

    void RegionRenderer::DrawNextLayer()
    {
        auto& layer = drawContext->currentLayer->second;

        Draw(layer);

        ++drawContext->currentLayer;
    }

    void RegionRenderer::End()
    {
        drawContext.reset();

        layers.Clear();
        descriptorSetPool.Reset();
    }

    void RegionRenderer::MaterialCreated(const Asset::Material& material)
    {
        if (material.Type() != Asset::MaterialType::Region)
            return;

        mappedConduits.Add(material);
    }

    void RegionRenderer::MaterialDestroying(const Asset::Material& material)
    {
        if (material.Type() != Asset::MaterialType::Region)
            return;

        mappedConduits.Remove(material);
    }

    bool RegionRenderer::IsDone() const
    {
        return !drawContext || drawContext->currentLayer == layers.end();
    }

    Spatial::Point3D::Value RegionRenderer::NextLayer() const
    {
        return drawContext->currentLayer->first;
    }

    size_t RegionRenderer::LayerCount() const
    {
        return layers.Count();
    }

    RegionRenderer::Region::Region(const Vertices& vertices, const Indices& indices) :
        vertices(vertices), indices(indices)
    {}

    void RegionRenderer::Draw(Layer& layer)
    {
        const auto& commandBuffer = drawContext->commandBuffer;

        vk::Buffer vertexBuffers[] = { vertexBuffer.destination.value.get() };
        vk::DeviceSize offsets[] = { 0 };
        commandBuffer.bindVertexBuffers(0, 1, vertexBuffers, offsets);

        commandBuffer.bindIndexBuffer(indexBuffer.destination.value.get(), 0, vk::IndexType::eUint16);

        for (auto& group : layer.materialGroups)
            WriteToBuffers(group.second, *group.first);
    }

    void RegionRenderer::WriteToBuffers(
        const Layer::MaterialGroup& group,
        const Asset::Material& materialAsset)
    {
        auto& conduits = *mappedConduits.For(materialAsset);
        for (auto& conduit : conduits)
            WriteToBuffers(conduit, group.values);
    }

    void RegionRenderer::WriteToBuffers(
        Conduit& conduit,
        const std::vector<Region>& regions)
    {
        const auto& commandBuffer = drawContext->commandBuffer;

        const auto startVertexCount = drawContext->vertexCount;
        const auto startIndexCount = drawContext->indexCount;

        std::vector<Vertex> drawnVertices;
        std::vector<Index> drawnIndices;
        for (auto& region : regions)
        {
            drawnVertices.insert(drawnVertices.end(), region.vertices.begin(), region.vertices.end());
            drawContext->vertexCount += region.vertices.size();

            drawnIndices.insert(drawnIndices.end(), region.indices.begin(), region.indices.end());
            drawContext->indexCount += region.indices.size();
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

        const auto vertexOffset = vk::DeviceSize(startVertexCount) * sizeof Vertex;
        const auto vertexSize = drawnVertices.size() * sizeof Vertex;
        const auto indexOffset = vk::DeviceSize(startIndexCount) * sizeof Index;
        const auto indexSize = drawnIndices.size() * sizeof Index;

        vertexBuffer.PushSourceBytes(drawnVertices.data(), vertexOffset, vertexSize);
        vertexBuffer.CopyFromSourceToDestination(vertexOffset, vertexSize, drawContext->commandPool, graphicsQueue);
        indexBuffer.PushSourceBytes(drawnIndices.data(), indexOffset, indexSize);
        indexBuffer.CopyFromSourceToDestination(indexOffset, indexSize, drawContext->commandPool, graphicsQueue);
        drawContext->commandBuffer.drawIndexed(drawnIndices.size(), 1, startIndexCount, 0, 0);
    }
}