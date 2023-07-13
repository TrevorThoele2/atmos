#include "VulkanRegionRenderer.h"

#include "VulkanCommandBuffer.h"
#include "VulkanMaxFramesInFlight.h"

#include "RasterCommand.h"

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

    void RegionRenderer::Start()
    {
        totalVertices = 0;
        totalIndices = 0;
    }

    std::optional<RendererPass> RegionRenderer::Draw(
        const std::vector<Raster::DrawRegion>& draws, const UniversalDataBuffer& universalDataBuffer)
    {
        if (std::find_if(draws.begin(), draws.end(), [](const Raster::Command& command) { return std::holds_alternative<Raster::DrawRegion>(command); }) == draws.end())
            return {};

        struct Group
        {
            Shaders shaders;
            std::vector<Region> elements = {};
            size_t totalIndices = 0;
        };

        std::vector<Group> groups;
        auto currentGroup = &groups.emplace_back(
            draws.begin()->shaders,
            std::vector<Region> {},
            0);
        for (auto& draw : draws)
        {
            if (draw.shaders != currentGroup->shaders)
            {
                currentGroup = &groups.emplace_back(
                    draw.shaders,
                    std::vector<Region>{},
                    0);
            }

            std::vector<Vertex> vertices;
            for (auto& point : draw.mesh.vertices)
                vertices.push_back(Vertex{ { point.x, point.y } });

            currentGroup->elements.emplace_back(vertices, draw.mesh.indices);
            currentGroup->totalIndices += draw.mesh.indices.size();
        }

        auto& descriptorSetPool = NextDescriptorSetPool();
        const auto descriptorSets = descriptorSetPool.Retrieve(groups.size());
        
        return RendererPass
        {
            Command
            {
                [this, groups, descriptorSets, &universalDataBuffer](CommandRecorder record)
                {
                    auto drawnIndices = 0;

                    size_t i = 0;
                    for (auto& group : groups)
                    {
                        const auto descriptorSet = descriptorSets[i];
                        
                        universalDataBuffer.Update(descriptorSet);

                        record(WriteData(group.elements, totalIndices + drawnIndices));

                        drawnIndices += group.totalIndices;
                        ++i;
                    }
                }
            },
            Command
            {
                [this, groups, descriptorSets](CommandRecorder record)
                {
                    auto drawnIndices = 0;

                    size_t i = 0;
                    for (auto& group : groups)
                    {
                        const auto descriptorSet = descriptorSets[i];

                        record(Draw(
                            totalIndices + drawnIndices,
                            group.totalIndices,
                            mappedConduits.For(group.shaders),
                            descriptorSet));

                        drawnIndices += group.totalIndices;
                        ++i;
                    }

                    totalIndices += drawnIndices;
                }
            }
        };
    }
    
    Command RegionRenderer::WriteData(
        const std::vector<Region>& regions, std::uint32_t startIndexCount)
    {
        const auto startVertexCount = totalVertices;

        std::vector<Vertex> verticesToDraw;
        std::vector<Index> indicesToDraw;
        for (auto& region : regions)
        {
            verticesToDraw.insert(verticesToDraw.end(), region.vertices.begin(), region.vertices.end());
            totalVertices += region.vertices.size();

            const auto startMaxIndex = maxIndex;
            indicesToDraw.reserve(indicesToDraw.size() + region.indices.size());
            for (auto& index : region.indices)
            {
                const Index useIndex = startMaxIndex + static_cast<Index>(index);
                indicesToDraw.push_back(useIndex);
                maxIndex = std::max(maxIndex, Index(useIndex + 1));
            }
        }

        const auto vertexOffset = vk::DeviceSize(startVertexCount) * sizeof Vertex;
        const auto vertexSize = verticesToDraw.size() * sizeof Vertex;
        const auto indexOffset = vk::DeviceSize(startIndexCount) * sizeof Index;
        const auto indexSize = indicesToDraw.size() * sizeof Index;

        vertexBuffer.PushSourceBytes(verticesToDraw.data(), vertexOffset, vertexSize);
        indexBuffer.PushSourceBytes(indicesToDraw.data(), indexOffset, indexSize);

        return [this, vertexOffset, vertexSize, indexOffset, indexSize](CommandRecorder record)
        {
            record(vertexBuffer.CopyFromSourceToDestination(vertexOffset, vertexSize));
            record(indexBuffer.CopyFromSourceToDestination(indexOffset, indexSize));
        };
    }

    Command RegionRenderer::Draw(
        std::uint32_t startIndexCount, std::uint32_t indexCount, Conduit& conduit, vk::DescriptorSet descriptorSet)
    {
        return [this, startIndexCount, indexCount, &conduit, descriptorSet](CommandRecorder record)
        {
            record(conduit.Bind());
            record([this, startIndexCount, indexCount, &conduit, descriptorSet](vk::CommandBuffer commandBuffer)
                {
                    const vk::Buffer vertexBuffers[] = { vertexBuffer.destination.Value() };
                    constexpr vk::DeviceSize offsets[] = { 0 };
                    commandBuffer.bindVertexBuffers(0, 1, vertexBuffers, offsets);
                    commandBuffer.bindIndexBuffer(indexBuffer.destination.Value(), 0, vk::IndexType::eUint16);

                    commandBuffer.bindDescriptorSets(
                        vk::PipelineBindPoint::eGraphics,
                        mappedConduits.PipelineLayout(),
                        0,
                        1,
                        &descriptorSet,
                        0,
                        nullptr);

                    commandBuffer.drawIndexed(indexCount, 1, startIndexCount, 0, 0);
                });
        };
    }

    std::vector<DescriptorSetPool> RegionRenderer::CreateDescriptorSetPools(vk::Device device)
    {
        std::vector<DescriptorSetPool> pools;
        for (size_t i = 0; i < maxFramesInFlight; ++i)
        {
            pools.emplace_back(
                std::vector
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