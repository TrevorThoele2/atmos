#include "VulkanLineRenderer.h"

#include "VulkanCommandBuffer.h"
#include "VulkanConduit.h"
#include "VulkanUtilities.h"
#include "VulkanMaxFramesInFlight.h"

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

    void LineRenderer::StageRender(const RenderLine& lineRender)
    {
        stagedLineRenders.push_back(lineRender);
    }
    
    std::unique_ptr<Raster> LineRenderer::Start(
        vk::CommandBuffer drawCommandBuffer,
        const UniversalDataBuffer& universalDataBuffer)
    {
        const auto totalLineCount = stagedLineRenders.size();
        if (totalLineCount == 0)
            return {};
        
        auto raster = std::make_unique<Raster>(*this);

        auto& descriptorSetPool = NextDescriptorSetPool();
        raster->setupDescriptorSet = descriptorSetPool.Retrieve(1)[0];

        universalDataBuffer.Update(raster->setupDescriptorSet);

        for (auto& stagedLineRender : stagedLineRenders)
        {
            mappedConduits.Add(stagedLineRender.material);
            AddToRaster(stagedLineRender, *raster);
        }
        stagedLineRenders.clear();

        raster->currentLayer = raster->layers.begin();
        return raster;
    }

    void LineRenderer::MaterialDestroying(Arca::Index<Asset::Material> material)
    {
        mappedConduits.Remove(material);
    }

    size_t LineRenderer::RenderCount() const
    {
        return stagedLineRenders.size();
    }

    LineRenderer::Line::Line(const std::vector<Vertex>& points) : vertices(points)
    {}

    LineRenderer::Raster::Raster(LineRenderer& renderer) : renderer(&renderer)
    {}

    auto LineRenderer::Raster::NextPasses() -> std::vector<Pass>
    {
        const auto& layer = currentLayer->second;
        auto passes = NextPasses(layer);
        ++currentLayer;
        return passes;
    }

    bool LineRenderer::Raster::IsDone() const
    {
        return currentLayer == layers.end();
    }

    ObjectLayeringKey LineRenderer::Raster::NextLayer() const
    {
        return currentLayer->first;
    }

    auto LineRenderer::Raster::NextPasses(const Layer& layer) -> std::vector<Pass>
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

    auto LineRenderer::Raster::NextPasses(const Layer::MaterialGroup& materialGroup, MappedConduits::Group& conduitGroup) -> std::vector<Pass>
    {
        std::vector<Pass> passes;
        for (auto& conduit : conduitGroup)
        {
            for (auto& value : materialGroup.values)
            {
                const auto startVertexCount = totalVertexCount;
                const auto lines = value.second;
                std::uint32_t currentVertexCount = 0;
                for (auto& line : lines)
                    currentVertexCount += line.vertices.size();
                passes.emplace_back(
                    WriteData(value.second, startVertexCount),
                    Draw(startVertexCount, currentVertexCount, conduit, value.first));
                totalVertexCount += currentVertexCount;
            }
        }
        return passes;
    }

    Command LineRenderer::Raster::WriteData(const std::vector<Line>& lines, std::uint32_t startVertexCount)
    {
        std::vector<Vertex> verticesToDraw;
        for (auto& line : lines)
        {
            verticesToDraw.insert(verticesToDraw.begin(), line.vertices.begin(), line.vertices.end());
            totalVertexCount += line.vertices.size();
        }

        const auto vertexOffset = vk::DeviceSize(startVertexCount) * sizeof Vertex;
        const auto vertexSize = verticesToDraw.size() * sizeof Vertex;

        renderer->vertexBuffer.PushSourceBytes(verticesToDraw.data(), vertexOffset, vertexSize);

        return [this, vertexOffset, vertexSize](CommandRecorder record)
        {
            record(renderer->vertexBuffer.CopyFromSourceToDestination(vertexOffset, vertexSize));
        };
    }
    
	Command LineRenderer::Raster::Draw(std::uint32_t startVertexCount, std::uint32_t vertexCount, Conduit& conduit, LineWidth lineWidth)
    {
        return [this, startVertexCount, vertexCount, &conduit, lineWidth](CommandRecorder record)
        {
            record(conduit.Bind());
            record([this, startVertexCount, vertexCount, &conduit, lineWidth](vk::CommandBuffer commandBuffer)
                {
                    const vk::Buffer vertexBuffers[] = { renderer->vertexBuffer.destination.Value() };
                    constexpr vk::DeviceSize offsets[] = { 0 };
                    commandBuffer.bindVertexBuffers(0, 1, vertexBuffers, offsets);

                    commandBuffer.bindDescriptorSets(
                        vk::PipelineBindPoint::eGraphics,
                        renderer->mappedConduits.PipelineLayout(),
                        0,
                        1,
                        &setupDescriptorSet,
                        0,
                        nullptr);

                    commandBuffer.setLineWidth(lineWidth);

                    commandBuffer.draw(vertexCount, 1, startVertexCount, 0);
                });
        };
    }

    void LineRenderer::AddToRaster(const RenderLine& lineRender, Raster& raster)
    {
        if (!lineRender.points.empty())
        {
            std::vector<Vertex> points;
            for (auto& point : lineRender.points)
            {
                const auto color = AtmosToVulkanColor(lineRender.color);
                points.push_back(Vertex{ color, { point.x, point.y } });
            }

            const auto key = ObjectLayeringKey{ lineRender.space, lineRender.z };
            auto layer = raster.layers.Find(key);
            if (!layer)
                layer = &raster.layers.Add(key, Raster::Layer{});
            auto& group = layer->GroupFor(lineRender.material.ID());
            group.ListFor(lineRender.width).emplace_back(points);
        }
    }

    std::vector<DescriptorSetPool> LineRenderer::CreateDescriptorSetPools(vk::Device device)
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

    DescriptorSetPool& LineRenderer::NextDescriptorSetPool()
    {
        ++currentDescriptorSetPool;
        if (currentDescriptorSetPool == descriptorSetPools.end())
            currentDescriptorSetPool = descriptorSetPools.begin();
        return *currentDescriptorSetPool;
    }
}