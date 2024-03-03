#include "VulkanLineRenderer.h"

#include "VulkanUtilities.h"

namespace Atmos::Render::Vulkan
{
    LineRenderer::LineRenderer(
        std::shared_ptr<vk::Device> device,
        vk::Queue graphicsQueue,
        vk::PhysicalDeviceMemoryProperties memoryProperties,
        vk::RenderPass renderPass,
        vk::Extent2D swapchainExtent,
        const Arca::Batch<Asset::Material>& materials)
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
            { descriptorSetPool.DescriptorSetLayout() }),
        graphicsQueue(graphicsQueue),
        device(device)
    {
        for (auto& material : materials)
            MaterialCreated(Arca::Index<Asset::Material>{material.ID(), material.Owner()});
    }

    void LineRenderer::StageRender(const LineRender& lineRender)
    {
        stagedLineRenders.push_back(lineRender);
    }

    std::unique_ptr<Raster> LineRenderer::Start(
        vk::CommandBuffer commandBuffer,
        vk::CommandPool commandPool,
        const UniversalDataBuffer& universalDataBuffer)
    {
        if (stagedLineRenders.empty())
            return {};

        auto raster = std::make_unique<Raster>(commandBuffer, commandPool, *this);

        descriptorSetPool.Reset();
        descriptorSetPool.Reserve(1);
        raster->setupDescriptorSet = descriptorSetPool.Next();

        universalDataBuffer.Update(raster->setupDescriptorSet);

        for (auto stagedLineRender = stagedLineRenders.begin(); stagedLineRender != stagedLineRenders.end();)
        {
            AddToRaster(*stagedLineRender, *raster);
            stagedLineRender = stagedLineRenders.erase(stagedLineRender);
        }

        raster->currentLayer = raster->layers.begin();
        return raster;
    }

    void LineRenderer::MaterialCreated(Arca::Index<Asset::Material> material)
    {
        if (material->Type() != Asset::MaterialType::Line)
            return;

        mappedConduits.Add(material);
    }

    void LineRenderer::MaterialDestroying(Arca::Index<Asset::Material> material)
    {
        if (material->Type() != Asset::MaterialType::Line)
            return;

        mappedConduits.Remove(material);
    }

    size_t LineRenderer::RenderCount() const
    {
        return stagedLineRenders.size();
    }

    LineRenderer::Line::Line(const std::vector<Vertex>& points) : vertices(points)
    {}

    LineRenderer::Raster::Raster(
        vk::CommandBuffer commandBuffer,
        vk::CommandPool commandPool,
        LineRenderer& renderer)
        :
        commandBuffer(commandBuffer),
        commandPool(commandPool),
        renderer(&renderer)
    {}

    void LineRenderer::Raster::DrawNextLayer()
    {
        auto& layer =currentLayer->second;

        Draw(layer);

        ++currentLayer;
    }

    bool LineRenderer::Raster::IsDone() const
    {
        return currentLayer == layers.end();
    }

    Spatial::Point3D::Value LineRenderer::Raster::NextLayer() const
    {
        return currentLayer->first;
    }

    void LineRenderer::Raster::Draw(Layer& layer)
    {
        vk::Buffer vertexBuffers[] = { renderer->vertexBuffer.destination.value.get() };
        vk::DeviceSize offsets[] = { 0 };
        commandBuffer.bindVertexBuffers(0, 1, vertexBuffers, offsets);

        for (auto& materialGroup : layer.materialGroups)
        {
            const auto conduitGroup = renderer->mappedConduits.For(materialGroup.first);
            if (!conduitGroup)
                return;

            WriteToBuffers(materialGroup.second, *conduitGroup);
        }
    }

    void LineRenderer::Raster::WriteToBuffers(
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

            for (auto& value : materialGroup.values)
            {
                commandBuffer.setLineWidth(value.first);
                WriteToBuffers(value.second);
            }
        }
    }

    void LineRenderer::Raster::WriteToBuffers(const std::vector<Line>& lines)
    {
        const auto startVertexCount = vertexCount;

        std::vector<Vertex> drawnVertices;
        for (auto& line : lines)
        {
            drawnVertices.insert(drawnVertices.begin(), line.vertices.begin(), line.vertices.end());
            vertexCount += line.vertices.size();
        }

        const auto lineVertexOffset = vk::DeviceSize(startVertexCount) * sizeof Vertex;
        const auto lineVertexSize = drawnVertices.size() * sizeof Vertex;

        renderer->vertexBuffer.PushSourceBytes(drawnVertices.data(), lineVertexOffset, lineVertexSize);
        renderer->vertexBuffer.CopyFromSourceToDestination(lineVertexOffset, lineVertexSize, commandPool, renderer->graphicsQueue);
        commandBuffer.draw(drawnVertices.size(), 1, startVertexCount, 0);
    }

    void LineRenderer::AddToRaster(const LineRender& lineRender, Raster& raster)
    {
        if (lineRender.points.empty())
            return;

        const auto materialAsset = lineRender.material;

        std::vector<Vertex> points;
        for (auto& point : lineRender.points)
        {
            const auto color = AtmosToVulkanColor(lineRender.color);
            points.push_back(Vertex{
                color,
                { point.x, point.y } });
        }

        auto layer = raster.layers.Find(lineRender.z);
        if (!layer)
            layer = &raster.layers.Add(lineRender.z, Raster::Layer{});
        auto& group = layer->GroupFor(materialAsset->ID());
        group.ListFor(lineRender.width).emplace_back(points);
    }
}