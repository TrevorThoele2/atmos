#include "VulkanQuadRenderer.h"

#include "VulkanUniversalData.h"
#include "VulkanImageAssetResource.h"
#include "VulkanUtilities.h"

#include "SpatialAlgorithms.h"

namespace Atmos::Render::Vulkan
{
    QuadRenderer::QuadRenderer(
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
                },
                DescriptorSetGroup::Definition
                {
                    vk::DescriptorType::eCombinedImageSampler,
                    1,
                    vk::ShaderStageFlagBits::eFragment,
                    1
                }
            },
            VertexInput(
                sizeof(Vertex),
                {
                    vk::VertexInputAttributeDescription(
                        0, 0, vk::Format::eR32G32Sfloat, offsetof(Vertex, position)),
                    vk::VertexInputAttributeDescription(
                        1, 0, vk::Format::eR32G32Sfloat, offsetof(Vertex, texture)),
                    vk::VertexInputAttributeDescription(
                        2, 0, vk::Format::eR32G32B32A32Sfloat, offsetof(Vertex, color))
                }),
            vk::PrimitiveTopology::eTriangleList,
            Asset::MaterialType::Image,
            [this](Context& context, DrawContext& drawContext, uint32_t currentImage, UniversalData universalData)
            {
                Draw(context, drawContext, currentImage, universalData);
            }),
        graphicsQueue(graphicsQueue),
        commandBuffers(*device, graphicsQueueIndex),
        device(device)
    {}

    void QuadRenderer::Initialize(uint32_t swapchainImageCount, vk::RenderPass renderPass, vk::Extent2D extent)
    {
        core.Initialize(swapchainImageCount, renderPass, extent);
    }

    void QuadRenderer::StageRender(const ImageRender& imageRender)
    {
        const auto rotate = [](
            const Spatial::Point2D& position,
            const Spatial::Angle2D& angle,
            const Spatial::Point2D& center)
        {
            const auto sinAngle = std::sin(angle);
            const auto cosAngle = std::cos(angle);

            return Spatial::Point2D
            {
                position.x * cosAngle - position.y * sinAngle + center.x,
                position.x * sinAngle + position.y * cosAngle + center.y
            };
        };

        const auto imageAsset = imageRender.asset;
        const auto slice = imageRender.assetSlice;
        const auto materialAsset = imageRender.material;
        const auto color = AtmosToVulkanColor(imageRender.color);

        const auto position = ToPoint2D(imageRender.position);
        const auto halfWidth = imageRender.size.width / 2;
        const auto halfHeight = imageRender.size.height / 2;
        const auto topLeft = rotate({ -halfWidth, -halfHeight }, imageRender.rotation, position);
        const auto topRight = rotate({ halfWidth, -halfHeight }, imageRender.rotation, position);
        const auto bottomLeft = rotate({ -halfWidth, halfHeight }, imageRender.rotation, position);
        const auto bottomRight = rotate({ halfWidth, halfHeight }, imageRender.rotation, position);

        const auto adjustedSliceLeft = slice.Left() / imageAsset->Width();
        const auto adjustedSliceTop = slice.Top() / imageAsset->Height();
        const auto adjustedSliceRight = slice.Right() / imageAsset->Width();
        const auto adjustedSliceBottom = slice.Bottom() / imageAsset->Height();

        const std::array<Vertex, 4> vertices
        {
            Vertex
            {
                color,
                { topLeft.x, topLeft.y },
                { adjustedSliceLeft, adjustedSliceTop }
            },
            Vertex
            {
                color,
                { topRight.x, topRight.y },
                { adjustedSliceRight, adjustedSliceTop }
            },
            Vertex
            {
                color,
                { bottomLeft.x, bottomLeft.y },
                { adjustedSliceLeft, adjustedSliceBottom }
            },
            Vertex
            {
                color,
                { bottomRight.x, bottomRight.y },
                { adjustedSliceRight, adjustedSliceBottom }
            }
        };
        auto context = core.ContextFor(imageRender.position.z);
        if (!context)
            context = &core.AddContext(imageRender.position.z, Context{});
        auto& group = context->GroupFor(*materialAsset);
        group.ListFor(*imageAsset).emplace_back(vertices);
        core.AddKey(imageAsset);
    }

    void QuadRenderer::Start(const std::vector<const Asset::Material*>& materials, vk::CommandBuffer commandBuffer)
    {
        const auto setupKey = [this](const Asset::Image* key, vk::DescriptorSet& descriptorSet)
        {
            const auto imageAssetResource = key->ResourceAs<Asset::Resource::Vulkan::Image>();
            const auto imageAssetDescriptor = imageAssetResource->descriptor;
            imageAssetDescriptor.Update(descriptorSet, *device);
        };
        core.Start(materials, commandBuffer, setupKey);
    }

    void QuadRenderer::DrawNextLayer(uint32_t currentImage, UniversalData universalData)
    {
        core.DrawNextLayer(currentImage, universalData);
    }

    void QuadRenderer::End()
    {
        core.End();
    }

    bool QuadRenderer::IsDone() const
    {
        return core.IsDone();
    }

    Spatial::Point3D::Value QuadRenderer::NextLayer() const
    {
        return core.NextLayer();
    }

    size_t QuadRenderer::LayerCount() const
    {
        return core.LayerCount();
    }

    QuadRenderer::Quad::Quad(const Vertices& vertices) : vertices(vertices)
    {}

    auto QuadRenderer::Context::Group::ListFor(const Asset::Image& image) -> std::vector<Quad>&
    {
        auto found = quads.find(&image);
        if (found == quads.end())
            return quads.emplace(&image, std::vector<Quad>{}).first->second;

        return found->second;
    }

    auto QuadRenderer::Context::GroupFor(const Asset::Material& material) -> Group&
    {
        auto found = groups.find(&material);
        if (found == groups.end())
            return groups.emplace(&material, Group{}).first->second;

        return found->second;
    }

    void QuadRenderer::Draw(
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

    void QuadRenderer::WriteToBuffers(
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

            for (auto& object : group.quads)
                WriteToBuffers(
                    object.second,
                    object.first,
                    pipeline,
                    drawContext,
                    currentImage);
        }
    }

    void QuadRenderer::WriteToBuffers(
        const std::vector<Quad>& quads,
        const Asset::Image* imageAsset,
        Pipeline& pipeline,
        DrawContext& drawContext,
        uint32_t currentImage)
    {
        const auto& commandBuffer = drawContext.commandBuffer;

        const auto startQuadCount = drawContext.addition.quadCount;

        std::vector<Vertex> drawnVertices;
        std::vector<Index> drawnIndices;
        for (auto& quad : quads)
        {
            drawnVertices.insert(drawnVertices.end(), quad.vertices.begin(), quad.vertices.end());
            const auto offsetQuadIndicesBy = drawContext.addition.quadCount * indexIncrement;
            const Indices offsetQuadIndices =
            {
                Index(offsetQuadIndicesBy + indices[0]),
                Index(offsetQuadIndicesBy + indices[1]),
                Index(offsetQuadIndicesBy + indices[2]),
                Index(offsetQuadIndicesBy + indices[3]),
                Index(offsetQuadIndicesBy + indices[4]),
                Index(offsetQuadIndicesBy + indices[5]),
            };
            drawnIndices.insert(drawnIndices.end(), offsetQuadIndices.begin(), offsetQuadIndices.end());
            ++drawContext.addition.quadCount;
        }

        commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline.value.get());

        auto currentDescriptorSet = core.KeyedSetFor(imageAsset, currentImage)->descriptorSet;
        commandBuffer.bindDescriptorSets(
            vk::PipelineBindPoint::eGraphics,
            core.PipelineLayout(),
            0,
            1,
            &currentDescriptorSet,
            0,
            nullptr);

        const auto vertexOffset = vk::DeviceSize(startQuadCount) * 4 * sizeof Vertex;
        const auto vertexSize = drawnVertices.size() * sizeof Vertex;
        const auto indexOffset = vk::DeviceSize(startQuadCount) * indices.size() * sizeof Index;
        const auto indexSize = drawnIndices.size() * sizeof Index;

        vertexBuffer.PushSourceBytes(drawnVertices.data(), vertexOffset, vertexSize);
        vertexBuffer.CopyFromSourceToDestination(vertexOffset, vertexSize, commandBuffers.Pool(), graphicsQueue);
        indexBuffer.PushSourceBytes(drawnIndices.data(), indexOffset, indexSize);
        indexBuffer.CopyFromSourceToDestination(indexOffset, indexSize, commandBuffers.Pool(), graphicsQueue);
        drawContext.commandBuffer.drawIndexed(quads.size() * indices.size(), 1, startQuadCount * indices.size(), 0, 0);
    }
}
