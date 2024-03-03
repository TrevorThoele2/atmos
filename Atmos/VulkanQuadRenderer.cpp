#include "VulkanQuadRenderer.h"

#include "VulkanUniversalData.h"
#include "VulkanImageAssetResource.h"
#include "VulkanUtilities.h"

#include "SpatialAlgorithms.h"

namespace Atmos::Render::Vulkan
{
    QuadRenderer::QuadRenderer(
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
                },
                DescriptorSetPool::Definition
                {
                    vk::DescriptorType::eCombinedImageSampler,
                    1,
                    vk::ShaderStageFlagBits::eFragment,
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
                        1, 0, vk::Format::eR32G32Sfloat, offsetof(Vertex, texture)),
                    vk::VertexInputAttributeDescription(
                        2, 0, vk::Format::eR32G32B32A32Sfloat, offsetof(Vertex, color))
                }),
            vk::PrimitiveTopology::eTriangleList,
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
        auto layer = layers.Find(imageRender.position.z);
        if (!layer)
            layer = &layers.Add(imageRender.position.z, Layer{});
        auto& group = layer->GroupFor(*materialAsset);
        group.ListFor(imageAsset).emplace_back(vertices);
        stagedImageAssets.emplace(imageAsset);
    }

    void QuadRenderer::Start(
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

        descriptorSetPool.Reserve(stagedImageAssets.size() * swapchainImageCount);

        for (auto& imageAsset : stagedImageAssets)
        {
            for (uint32_t i = 0; i < swapchainImageCount; ++i)
            {
                drawContext->setupDescriptorSets.emplace_back(imageAsset, i, descriptorSetPool.Next());
                auto& emplaced = drawContext->setupDescriptorSets.back();
                const auto imageAssetResource = imageAsset->ResourceAs<Asset::Resource::Vulkan::Image>();
                const auto imageAssetDescriptor = imageAssetResource->descriptor;
                imageAssetDescriptor.Update(emplaced.value, *device);
            }
        }
    }

    void QuadRenderer::DrawNextLayer()
    {
        auto& layer = drawContext->currentLayer->second;

        Draw(layer);

        ++drawContext->currentLayer;
    }

    void QuadRenderer::End()
    {
        drawContext.reset();

        layers.Clear();
        descriptorSetPool.Reset();
        stagedImageAssets.clear();
    }

    void QuadRenderer::MaterialCreated(const Asset::Material& material)
    {
        if (material.Type() != Asset::MaterialType::Image)
            return;

        mappedConduits.Add(material);
    }

    void QuadRenderer::MaterialDestroying(const Asset::Material& material)
    {
        if (material.Type() != Asset::MaterialType::Image)
            return;

        mappedConduits.Remove(material);
    }

    bool QuadRenderer::IsDone() const
    {
        return !drawContext || drawContext->currentLayer == layers.end();
    }

    Spatial::Point3D::Value QuadRenderer::NextLayer() const
    {
        return drawContext->currentLayer->first;
    }

    size_t QuadRenderer::LayerCount() const
    {
        return layers.Count();
    }

    QuadRenderer::Quad::Quad(const Vertices& vertices) : vertices(vertices)
    {}

    void QuadRenderer::Draw(Layer& layer)
    {
        const auto& commandBuffer = drawContext->commandBuffer;

        vk::Buffer vertexBuffers[] = { vertexBuffer.destination.value.get() };
        vk::DeviceSize offsets[] = { 0 };
        commandBuffer.bindVertexBuffers(0, 1, vertexBuffers, offsets);

        commandBuffer.bindIndexBuffer(indexBuffer.destination.value.get(), 0, vk::IndexType::eUint16);

        for (auto& group : layer.materialGroups)
            WriteToBuffers(group.second, *group.first);
    }

    void QuadRenderer::WriteToBuffers(
        const Layer::MaterialGroup& materialGroup,
        const Asset::Material& materialAsset)
    {
        auto& conduits = *mappedConduits.For(materialAsset);
        for (auto& conduit : conduits)
            for (auto& value : materialGroup.values)
                WriteToBuffers(conduit, value.second, value.first);
    }

    void QuadRenderer::WriteToBuffers(
        Conduit& conduit,
        const std::vector<Quad>& quads,
        const Asset::Image* imageAsset)
    {
        const auto& commandBuffer = drawContext->commandBuffer;

        const auto startQuadCount = drawContext->quadCount;

        std::vector<Vertex> drawnVertices;
        std::vector<Index> drawnIndices;
        for (auto& quad : quads)
        {
            drawnVertices.insert(drawnVertices.end(), quad.vertices.begin(), quad.vertices.end());
            const auto offsetQuadIndicesBy = drawContext->quadCount * indexIncrement;
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
            ++drawContext->quadCount;
        }

        auto currentDescriptorSet = std::find_if(
            drawContext->setupDescriptorSets.begin(),
            drawContext->setupDescriptorSets.end(),
            [this, imageAsset](const SetupDescriptorSet& set)
            {
                return set.image == imageAsset && set.swapchainImage == drawContext->currentSwapchainImage;
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

        const auto vertexOffset = vk::DeviceSize(startQuadCount) * 4 * sizeof Vertex;
        const auto vertexSize = drawnVertices.size() * sizeof Vertex;
        const auto indexOffset = vk::DeviceSize(startQuadCount) * indices.size() * sizeof Index;
        const auto indexSize = drawnIndices.size() * sizeof Index;

        vertexBuffer.PushSourceBytes(drawnVertices.data(), vertexOffset, vertexSize);
        vertexBuffer.CopyFromSourceToDestination(vertexOffset, vertexSize, drawContext->commandPool, graphicsQueue);
        indexBuffer.PushSourceBytes(drawnIndices.data(), indexOffset, indexSize);
        indexBuffer.CopyFromSourceToDestination(indexOffset, indexSize, drawContext->commandPool, graphicsQueue);
        drawContext->commandBuffer.drawIndexed(quads.size() * indices.size(), 1, startQuadCount * indices.size(), 0, 0);
    }
}
