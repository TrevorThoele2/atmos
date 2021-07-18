#include "VulkanRegionRenderer.h"

namespace Atmos::Render::Vulkan
{
    RegionRenderer::RegionRenderer(
        vk::Device device,
        vk::Queue graphicsQueue,
        vk::PhysicalDeviceMemoryProperties memoryProperties,
        vk::RenderPass renderPass,
        vk::Extent2D swapchainExtent)
        :
        vertexBuffer(vertexStride * sizeof(Vertex), device, memoryProperties, vk::BufferUsageFlagBits::eVertexBuffer),
        indexBuffer(indexStride * sizeof(Index), device, memoryProperties, vk::BufferUsageFlagBits::eIndexBuffer),
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
            VertexInput(
                sizeof(Vertex),
                {
                    vk::VertexInputAttributeDescription(
                        0, 0, vk::Format::eR32G32Sfloat, offsetof(Vertex, position))
                }),
            vk::PrimitiveTopology::eTriangleList,
            renderPass,
            swapchainExtent,
            {},
            { descriptorSetPool.DescriptorSetLayout() }),
        graphicsQueue(graphicsQueue),
        device(device)
    {}

    void RegionRenderer::StageRender(const RegionRender& regionRender)
    {
        stagedRegionRenders.push_back(regionRender);
    }

    std::unique_ptr<Raster> RegionRenderer::Start(
        vk::CommandBuffer commandBuffer,
        vk::CommandPool commandPool,
        const UniversalDataBuffer& universalDataBuffer)
    {
        if (stagedRegionRenders.empty())
            return {};

        auto raster = std::make_unique<Raster>(commandBuffer, commandPool, *this);

        descriptorSetPool.Reset();
        descriptorSetPool.Reserve(1);
        raster->setupDescriptorSet = descriptorSetPool.Next();

        universalDataBuffer.Update(raster->setupDescriptorSet);

        for (auto stagedRegionRender : stagedRegionRenders)
        {
            mappedConduits.Add(stagedRegionRender.material);
            AddToRaster(stagedRegionRender, *raster);
        }
        stagedRegionRenders.clear();

        raster->currentLayer = raster->layers.begin();
        return raster;
    }
    
    void RegionRenderer::MaterialDestroying(Arca::Index<Asset::Material> material)
    {
        mappedConduits.Remove(material);
    }

    size_t RegionRenderer::RenderCount() const
    {
        return stagedRegionRenders.size();
    }

    RegionRenderer::Region::Region(const Vertices& vertices, const Indices& indices) :
        vertices(vertices), indices(indices)
    {}

    RegionRenderer::Raster::Raster(
        vk::CommandBuffer commandBuffer,
        vk::CommandPool commandPool,
        RegionRenderer& renderer)
        :
        commandBuffer(commandBuffer),
        commandPool(commandPool),
        renderer(&renderer)
    {}

    void RegionRenderer::Raster::DrawNextLayer()
    {
        auto& layer = currentLayer->second;

        Draw(layer);

        ++currentLayer;
    }

    bool RegionRenderer::Raster::IsDone() const
    {
        return currentLayer == layers.end();
    }

    ObjectLayeringKey RegionRenderer::Raster::NextLayer() const
    {
        return currentLayer->first;
    }

    void RegionRenderer::Raster::Draw(Layer& layer)
    {
        vk::Buffer vertexBuffers[] = { renderer->vertexBuffer.destination.value.get() };
        vk::DeviceSize offsets[] = { 0 };
        commandBuffer.bindVertexBuffers(0, 1, vertexBuffers, offsets);

        commandBuffer.bindIndexBuffer(renderer->indexBuffer.destination.value.get(), 0, vk::IndexType::eUint16);

        for (auto& materialGroup : layer.materialGroups)
        {
            const auto conduitGroup = renderer->mappedConduits.For(materialGroup.first);
            if (!conduitGroup)
                return;

            WriteToBuffers(materialGroup.second, *conduitGroup);
        }
    }

    void RegionRenderer::Raster::WriteToBuffers(
        const Layer::MaterialGroup& materialGroup, MappedConduits::Group& conduitGroup)
    {
        for (auto& conduit : conduitGroup)
        {
            conduit.Bind(commandBuffer);
            commandBuffer.bindDescriptorSets(
                vk::PipelineBindPoint::eGraphics,
                renderer->mappedConduits.PipelineLayout(),
                0,
                1,
                &setupDescriptorSet,
                0,
                nullptr);
            WriteToBuffers(materialGroup.values);
        }
    }

    void RegionRenderer::Raster::WriteToBuffers(const std::vector<Region>& regions)
    {
        const auto startVertexCount = vertexCount;
        const auto startIndexCount = indexCount;

        std::vector<Vertex> drawnVertices;
        std::vector<Index> drawnIndices;
        for (auto& region : regions)
        {
            drawnVertices.insert(drawnVertices.end(), region.vertices.begin(), region.vertices.end());
            vertexCount += region.vertices.size();

            const auto startMaxIndex = maxIndex;
            drawnIndices.reserve(drawnIndices.size() + region.indices.size());
            for (auto& index : region.indices)
            {
                const auto useIndex = startMaxIndex + index;
                drawnIndices.push_back(useIndex);
                maxIndex = std::max(maxIndex, Index(useIndex + 1));
            }
            indexCount += region.indices.size();
        }

        const auto vertexOffset = vk::DeviceSize(startVertexCount) * sizeof Vertex;
        const auto vertexSize = drawnVertices.size() * sizeof Vertex;
        const auto indexOffset = vk::DeviceSize(startIndexCount) * sizeof Index;
        const auto indexSize = drawnIndices.size() * sizeof Index;

        renderer->vertexBuffer.PushSourceBytes(drawnVertices.data(), vertexOffset, vertexSize);
        renderer->vertexBuffer.CopyFromSourceToDestination(vertexOffset, vertexSize, commandPool, renderer->graphicsQueue);
        renderer->indexBuffer.PushSourceBytes(drawnIndices.data(), indexOffset, indexSize);
        renderer->indexBuffer.CopyFromSourceToDestination(indexOffset, indexSize, commandPool, renderer->graphicsQueue);
        commandBuffer.drawIndexed(drawnIndices.size(), 1, startIndexCount, 0, 0);
    }

    void RegionRenderer::AddToRaster(const RegionRender& regionRender, Raster& raster)
    {
        if (!regionRender.mesh.vertices.empty() && !regionRender.mesh.indices.empty())
        {
            std::vector<Vertex> vertices;
            for (auto& point : regionRender.mesh.vertices)
                vertices.push_back(Vertex{ { point.x, point.y } });

            const auto key = ObjectLayeringKey{ regionRender.space, regionRender.z };
            auto context = raster.layers.Find(key);
            if (!context)
                context = &raster.layers.Add(key, Raster::Layer{});
            auto& group = context->GroupFor(regionRender.material.ID());
            group.values.emplace_back(vertices, regionRender.mesh.indices);
        }
    }
}