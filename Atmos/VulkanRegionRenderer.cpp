#include "VulkanRegionRenderer.h"

#include "VulkanCommandBuffer.h"
#include "VulkanMaxFramesInFlight.h"

namespace Atmos::Render::Vulkan
{
    RegionRenderer::RegionRenderer(
        vk::Device device,
        vk::Queue graphicsQueue,
        vk::PhysicalDeviceMemoryProperties memoryProperties,
        vk::RenderPass renderPass,
        vk::Extent2D swapchainExtent)
        :
        memoryPool(0, memoryProperties, device),
        vertexBuffer(vertexStride * sizeof(Vertex), device, memoryPool, vk::BufferUsageFlagBits::eVertexBuffer),
        indexBuffer(indexStride * sizeof(Index), device, memoryPool, vk::BufferUsageFlagBits::eIndexBuffer),
        descriptorSetPools(CreateDescriptorSetPools(device)),
        currentDescriptorSetPool(descriptorSetPools.begin()),
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
            { descriptorSetPools[0].DescriptorSetLayout()}),
        graphicsQueue(graphicsQueue),
        device(device)
    {}

    void RegionRenderer::StageRender(const RenderRegion& regionRender)
    {
        stagedRegionRenders.push_back(regionRender);
    }
    
    std::unique_ptr<Raster> RegionRenderer::Start(
        vk::CommandBuffer drawCommandBuffer,
        const UniversalDataBuffer& universalDataBuffer)
    {
        const auto totalRegionCount = stagedRegionRenders.size();
        if (totalRegionCount == 0)
            return {};
        
        auto raster = std::make_unique<Raster>(*this);

        auto& descriptorSetPool = NextDescriptorSetPool();
        raster->setupDescriptorSet = descriptorSetPool.Retrieve(1)[0];

        universalDataBuffer.Update(raster->setupDescriptorSet);

        for (const auto& stagedRegionRender : stagedRegionRenders)
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

    RegionRenderer::Raster::Raster(RegionRenderer& renderer) : renderer(&renderer)
    {}
    
    auto RegionRenderer::Raster::NextPasses() -> std::vector<Pass>
    {
        const auto& layer = currentLayer->second;
        auto passes = NextPasses(layer);
        ++currentLayer;
        return passes;
    }

    bool RegionRenderer::Raster::IsDone() const
    {
        return currentLayer == layers.end();
    }

    ObjectLayeringKey RegionRenderer::Raster::NextLayer() const
    {
        return currentLayer->first;
    }
    
    auto RegionRenderer::Raster::NextPasses(const Layer& layer) -> std::vector<Pass>
    {
        std::vector<Pass> passes;
        for (auto& materialGroup : layer.materialGroups)
        {
            const auto conduitGroup = renderer->mappedConduits.For(materialGroup.first);
            if (conduitGroup)
            {
                const auto nextPasses = NextPasses(materialGroup.second, *conduitGroup);
                passes.insert(passes.end(), nextPasses.begin(), nextPasses.end());
            }
        }
        return passes;
    }

    auto RegionRenderer::Raster::NextPasses(const Layer::MaterialGroup& materialGroup, MappedConduits::Group& conduitGroup) -> std::vector<Pass>
    {
        std::vector<Pass> passes;
        for (auto& conduit : conduitGroup)
        {
            const auto startIndexCount = totalIndexCount;
            const auto regions = materialGroup.values;
            std::uint32_t currentIndexCount = 0;
            for (auto& region : regions)
                currentIndexCount += region.indices.size();
            passes.emplace_back(WriteData(materialGroup.values, startIndexCount), Draw(startIndexCount, currentIndexCount, conduit));
            totalIndexCount += currentIndexCount;
        }
        return passes;
    }

    Command RegionRenderer::Raster::WriteData(const std::vector<Region>& regions, std::uint32_t startIndexCount)
    {
        const auto startVertexCount = totalVertexCount;

        std::vector<Vertex> verticesToDraw;
        std::vector<Index> indicesToDraw;
        for (auto& region : regions)
        {
            verticesToDraw.insert(verticesToDraw.end(), region.vertices.begin(), region.vertices.end());
            totalVertexCount += region.vertices.size();

            const auto startMaxIndex = maxIndex;
            indicesToDraw.reserve(indicesToDraw.size() + region.indices.size());
            for (auto& index : region.indices)
            {
                const auto useIndex = startMaxIndex + index;
                indicesToDraw.push_back(useIndex);
                maxIndex = std::max(maxIndex, Index(useIndex + 1));
            }
        }

        const auto vertexOffset = vk::DeviceSize(startVertexCount) * sizeof Vertex;
        const auto vertexSize = verticesToDraw.size() * sizeof Vertex;
        const auto indexOffset = vk::DeviceSize(startIndexCount) * sizeof Index;
        const auto indexSize = indicesToDraw.size() * sizeof Index;

        renderer->vertexBuffer.PushSourceBytes(verticesToDraw.data(), vertexOffset, vertexSize);
        renderer->indexBuffer.PushSourceBytes(indicesToDraw.data(), indexOffset, indexSize);

        return [this, vertexOffset, vertexSize, indexOffset, indexSize](CommandRecorder record)
        {
            record(renderer->vertexBuffer.CopyFromSourceToDestination(vertexOffset, vertexSize));
            record(renderer->indexBuffer.CopyFromSourceToDestination(indexOffset, indexSize));
        };
    }

    Command RegionRenderer::Raster::Draw(std::uint32_t startIndexCount, std::uint32_t indexCount, Conduit& conduit)
    {
        return [this, startIndexCount, indexCount, &conduit](CommandRecorder record)
        {
            record(conduit.Bind());
            record([this, startIndexCount, indexCount, &conduit](vk::CommandBuffer commandBuffer)
                {
                    const vk::Buffer vertexBuffers[] = { renderer->vertexBuffer.destination.Value() };
                    constexpr vk::DeviceSize offsets[] = { 0 };
                    commandBuffer.bindVertexBuffers(0, 1, vertexBuffers, offsets);
                    commandBuffer.bindIndexBuffer(renderer->indexBuffer.destination.Value(), 0, vk::IndexType::eUint16);

                    commandBuffer.bindDescriptorSets(
                        vk::PipelineBindPoint::eGraphics,
                        renderer->mappedConduits.PipelineLayout(),
                        0,
                        1,
                        &setupDescriptorSet,
                        0,
                        nullptr);

                    commandBuffer.drawIndexed(indexCount, 1, startIndexCount, 0, 0);
                });
        };
    }

    void RegionRenderer::AddToRaster(const RenderRegion& regionRender, Raster& raster)
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

    std::vector<DescriptorSetPool> RegionRenderer::CreateDescriptorSetPools(vk::Device device)
    {
        std::vector<DescriptorSetPool> pools;
        for (size_t i = 0; i < maxFramesInFlight; ++i)
        {
            pools.emplace_back(
                std::vector<DescriptorSetPool::Definition>
                {
                    DescriptorSetPool::Definition
                    {
                        vk::DescriptorType::eUniformBuffer,
                        0,
                        vk::ShaderStageFlagBits::eFragment | vk::ShaderStageFlagBits::eVertex,
                        1
                    }
                },
                device);
        }
        return pools;
    }

    DescriptorSetPool& RegionRenderer::NextDescriptorSetPool()
    {
        ++currentDescriptorSetPool;
        if (currentDescriptorSetPool == descriptorSetPools.end())
            currentDescriptorSetPool = descriptorSetPools.begin();
        return *currentDescriptorSetPool;
    }
}