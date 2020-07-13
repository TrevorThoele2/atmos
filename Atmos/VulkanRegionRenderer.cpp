#include "VulkanRegionRenderer.h"

#include "VulkanUniversalData.h"

namespace Atmos::Render::Vulkan
{
    RegionRenderer::RegionRenderer(
        std::shared_ptr<vk::Device> device,
        uint32_t graphicsQueueIndex,
        vk::Queue graphicsQueue,
        vk::PhysicalDeviceMemoryProperties memoryProperties)
        :
        vertexBuffer(vertexStride * sizeof(Vertex), *device, memoryProperties, vk::BufferUsageFlagBits::eVertexBuffer),
        indexBuffer(indexStride * sizeof(Index), *device, memoryProperties, vk::BufferUsageFlagBits::eIndexBuffer),
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
                        0, 0, vk::Format::eR32G32Sfloat, offsetof(Vertex, position))
                }),
            vk::PrimitiveTopology::eTriangleList,
            Asset::MaterialType::Region,
            [this](Context& context, DrawContext& drawContext, uint32_t currentImage, UniversalData universalData)
            {
                Draw(context, drawContext, currentImage, universalData);
            }),
        graphicsQueue(graphicsQueue),
        commandBuffers(*device, graphicsQueueIndex),
        device(device)
    {}

    void RegionRenderer::Initialize(uint32_t swapchainImageCount, vk::RenderPass renderPass, vk::Extent2D extent)
    {
        core.Initialize(swapchainImageCount, renderPass, extent);
    }

    void RegionRenderer::StageRender(const RegionRender& regionRender)
    {
        if (regionRender.mesh.vertices.empty() || regionRender.mesh.indices.empty())
            return;

        std::vector<Vertex> vertices;
        for (auto& point : regionRender.mesh.vertices)
            vertices.push_back(Vertex{ { point.x, point.y } });

        auto context = core.ContextFor(regionRender.z);
        if (!context)
            context = &core.AddContext(regionRender.z, Context{});
        auto& group = context->GroupFor(*regionRender.material);
        group.regions.emplace_back(vertices, regionRender.mesh.indices);
    }

    void RegionRenderer::Start(const std::vector<const Asset::Material*>& materials, vk::CommandBuffer commandBuffer)
    {
        core.Start(materials, commandBuffer);
    }

    void RegionRenderer::DrawNextLayer(uint32_t currentImage, UniversalData universalData)
    {
        core.DrawNextLayer(currentImage, universalData);
    }

    void RegionRenderer::End()
    {
        core.End();
    }

    bool RegionRenderer::IsDone() const
    {
        return core.IsDone();
    }

    Spatial::Point3D::Value RegionRenderer::NextLayer() const
    {
        return core.NextLayer();
    }

    size_t RegionRenderer::LayerCount() const
    {
        return core.LayerCount();
    }

    RegionRenderer::Region::Region(const Vertices& vertices, const Indices& indices) :
        vertices(vertices), indices(indices)
    {}

    auto RegionRenderer::Context::GroupFor(const Asset::Material& material) -> Group&
    {
        auto found = groups.find(&material);
        if (found == groups.end())
            return groups.emplace(&material, Group{}).first->second;

        return found->second;
    }

    void RegionRenderer::Draw(
        Context& context,
        DrawContext& drawContext,
        uint32_t currentImage,
        UniversalData universalData)
    {
        const auto& commandBuffer = drawContext.commandBuffer;

        vk::Buffer vertexBuffers[] = { vertexBuffer.destination.value.get() };
        vk::DeviceSize offsets[] = { 0 };
        commandBuffer.bindVertexBuffers(0, 1, vertexBuffers, offsets);

        commandBuffer.bindIndexBuffer(indexBuffer.destination.value.get(), 0, vk::IndexType::eUint16);

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

    void RegionRenderer::WriteToBuffers(
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

            WriteToBuffers(
                group.regions,
                pipeline,
                drawContext,
                currentImage);
        }
    }

    void RegionRenderer::WriteToBuffers(
        const std::vector<Region>& regions,
        Pipeline& pipeline,
        DrawContext& drawContext,
        uint32_t currentImage)
    {
        const auto& commandBuffer = drawContext.commandBuffer;

        const auto startVertexCount = drawContext.addition.vertexCount;
        const auto startIndexCount = drawContext.addition.indexCount;

        std::vector<Vertex> drawnVertices;
        std::vector<Index> drawnIndices;
        for (auto& region : regions)
        {
            drawnVertices.insert(drawnVertices.end(), region.vertices.begin(), region.vertices.end());
            drawContext.addition.vertexCount += region.vertices.size();

            drawnIndices.insert(drawnIndices.end(), region.indices.begin(), region.indices.end());
            drawContext.addition.indexCount += region.indices.size();
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

        const auto vertexOffset = vk::DeviceSize(startVertexCount) * sizeof Vertex;
        const auto vertexSize = drawnVertices.size() * sizeof Vertex;
        const auto indexOffset = vk::DeviceSize(startIndexCount) * sizeof Index;
        const auto indexSize = drawnIndices.size() * sizeof Index;

        vertexBuffer.PushSourceBytes(drawnVertices.data(), vertexOffset, vertexSize);
        vertexBuffer.CopyFromSourceToDestination(vertexOffset, vertexSize, commandBuffers.Pool(), graphicsQueue);
        indexBuffer.PushSourceBytes(drawnIndices.data(), indexOffset, indexSize);
        indexBuffer.CopyFromSourceToDestination(indexOffset, indexSize, commandBuffers.Pool(), graphicsQueue);
        drawContext.commandBuffer.drawIndexed(drawnIndices.size(), 1, startIndexCount, 0, 0);
    }
}