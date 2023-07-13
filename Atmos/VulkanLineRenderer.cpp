#include "VulkanLineRenderer.h"

#include "VulkanCommandBuffer.h"
#include "VulkanConduit.h"
#include "VulkanUtilities.h"
#include "VulkanMaxFramesInFlight.h"
#include "RasterCommand.h"

namespace Atmos::Render::Vulkan
{
    LineRenderer::LineRenderer(
        vk::Device device,
        vk::Queue graphicsQueue,
        vk::PhysicalDeviceMemoryProperties memoryProperties,
        vk::RenderPass renderPass,
        vk::Extent2D swapchainExtent)
        :
        memoryPool(0, memoryProperties, device),
        vertexBuffer(vertexStride * sizeof(Vertex), device, memoryPool, vk::BufferUsageFlagBits::eVertexBuffer),
        descriptorSetPools(CreateDescriptorSetPools(device)),
        currentDescriptorSetPool(descriptorSetPools.begin()),
        mappedConduits(
            device,
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
            swapchainExtent,
            { vk::DynamicState::eLineWidth },
            { descriptorSetPools[0].DescriptorSetLayout()}),
        graphicsQueue(graphicsQueue),
        device(device)
    {}

    void LineRenderer::Start()
    {
        totalVertexCount = 0;
    }

    std::optional<RendererPass> LineRenderer::Draw(
        const std::vector<Raster::DrawLine>& draws, const UniversalDataBuffer& universalDataBuffer)
    {
        if (std::find_if(draws.begin(), draws.end(), [](const Raster::Command& command) { return std::holds_alternative<Raster::DrawLine>(command); }) == draws.end())
            return {};

        struct Group
        {
            LineWidth width = 0;
            Shaders shaders;
            std::vector<Line> lines = {};
            size_t vertexCount = 0;
        };

        std::vector<Group> groups;
        auto currentGroup = &groups.emplace_back(
            draws.begin()->width,
            draws.begin()->shaders,
            std::vector<Line> {},
            0);
        for (auto& draw : draws)
        {
            if (draw.width != currentGroup->width || draw.shaders != currentGroup->shaders)
            {
                currentGroup = &groups.emplace_back(
                    draw.width,
                    draw.shaders,
                    std::vector<Line>{},
                    0);
            }

            std::vector<Vertex> points;
            for (auto& point : draw.points)
            {
                const auto color = ToVulkanColor(draw.color);
                points.push_back(Vertex{ color, { point.x, point.y } });
            }
            
            currentGroup->lines.push_back(Line{ points });
            currentGroup->vertexCount += points.size();
        }

        auto& descriptorSetPool = NextDescriptorSetPool();
        const auto descriptorSet = descriptorSetPool.Retrieve(1)[0];

        return RendererPass
        {
            Command
            {
                [this, groups, descriptorSet, &universalDataBuffer](CommandRecorder record)
                {
                    auto drawnVertices = 0;

                    size_t i = 0;
                    for (auto& group : groups)
                    {
                        universalDataBuffer.Update(descriptorSet);

                        record(WriteData(group.lines, totalVertexCount + drawnVertices));
                        
                        drawnVertices += group.vertexCount;
                        ++i;
                    }
                }
            },
            Command
            {
                [this, groups, descriptorSet](CommandRecorder record)
                {
                    auto drawnVertices = 0;

                    size_t i = 0;
                    for (auto& group : groups)
                    {
                        record(Draw(
                            group.vertexCount,
                            totalVertexCount + drawnVertices,
                            mappedConduits.For(group.shaders),
                            group.width,
                            descriptorSet));
                        
                        drawnVertices += group.vertexCount;
                        ++i;
                    }

                    totalVertexCount += drawnVertices;
                }
            }
        };
    }
    
    Command LineRenderer::WriteData(
        const std::vector<Line>& lines, std::uint32_t startVertexCount)
    {
        std::vector<Vertex> verticesToDraw;
        for (auto& line : lines)
        {
            verticesToDraw.insert(verticesToDraw.begin(), line.vertices.begin(), line.vertices.end());
            totalVertexCount += line.vertices.size();
        }

        const auto vertexOffset = vk::DeviceSize(startVertexCount) * sizeof Vertex;
        const auto vertexSize = verticesToDraw.size() * sizeof Vertex;

        vertexBuffer.PushSourceBytes(verticesToDraw.data(), vertexOffset, vertexSize);

        return [this, vertexOffset, vertexSize](CommandRecorder record)
        {
            record(vertexBuffer.CopyFromSourceToDestination(vertexOffset, vertexSize));
        };
    }

    Command LineRenderer::Draw(
        std::uint32_t vertexCount, std::uint32_t startVertexCount, Conduit& conduit, LineWidth lineWidth, vk::DescriptorSet descriptorSet)
    {
        return [this, startVertexCount, vertexCount, &conduit, lineWidth, descriptorSet](CommandRecorder record)
        {
            record(conduit.Bind());
            record([this, startVertexCount, vertexCount, &conduit, lineWidth, descriptorSet](vk::CommandBuffer commandBuffer)
                {
                    const vk::Buffer vertexBuffers[] = { vertexBuffer.destination.Value() };
                    constexpr vk::DeviceSize offsets[] = { 0 };
                    commandBuffer.bindVertexBuffers(0, 1, vertexBuffers, offsets);

                    commandBuffer.bindDescriptorSets(
                        vk::PipelineBindPoint::eGraphics,
                        mappedConduits.PipelineLayout(),
                        0,
                        1,
                        &descriptorSet,
                        0,
                        nullptr);

                    commandBuffer.setLineWidth(lineWidth);

                    commandBuffer.draw(vertexCount, 1, startVertexCount, 0);
                });
        };
    }

    std::vector<DescriptorSetPool> LineRenderer::CreateDescriptorSetPools(vk::Device device)
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

    DescriptorSetPool& LineRenderer::NextDescriptorSetPool()
    {
        ++currentDescriptorSetPool;
        if (currentDescriptorSetPool == descriptorSetPools.end())
            currentDescriptorSetPool = descriptorSetPools.begin();
        return *currentDescriptorSetPool;
    }
}