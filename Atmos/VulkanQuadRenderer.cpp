#include "VulkanQuadRenderer.h"

#include "VulkanImageAssetResource.h"
#include "VulkanUtilities.h"

#include "SpatialAlgorithms.h"

namespace Atmos::Render::Vulkan
{
    QuadRendererDescriptorSetKey::QuadRendererDescriptorSetKey(const Asset::Image& image) :
        image(&image)
    {}

    bool QuadRendererDescriptorSetKey::operator==(const QuadRendererDescriptorSetKey& arg) const
    {
        return image == arg.image;
    }

    QuadRenderer::QuadRenderer(
        std::shared_ptr<vk::Device> device,
        vk::Queue graphicsQueue,
        vk::PhysicalDeviceMemoryProperties memoryProperties,
        vk::RenderPass renderPass,
        vk::Extent2D swapchainExtent,
        const Arca::Batch<Asset::ImageMaterial>& materials)
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
            swapchainExtent,
            { descriptorSetPool.DescriptorSetLayout() }),
        graphicsQueue(graphicsQueue),
        device(device)
    {
        for (auto material = materials.begin(); material != materials.end(); ++material)
            MaterialCreated(Arca::Index<Asset::ImageMaterial>{material.ID(), materials.Owner()});
    }

    void QuadRenderer::StageRender(const ImageRender& imageRender)
    {
        stagedImageRenders.push_back(imageRender);
    }

    std::unique_ptr<Raster> QuadRenderer::Start(
        vk::CommandBuffer commandBuffer,
        vk::CommandPool commandPool,
        const UniversalDataBuffer& universalDataBuffer)
    {
        if (stagedImageRenders.empty())
            return {};

        auto raster = std::make_unique<Raster>(commandBuffer, commandPool, *this);

        for (auto stagedImageRender = stagedImageRenders.begin(); stagedImageRender != stagedImageRenders.end();)
        {
            AddToRaster(*stagedImageRender, *raster);
            stagedImageRender = stagedImageRenders.erase(stagedImageRender);
        }

        descriptorSetPool.Reset();
        descriptorSetPool.Reserve(descriptorSetKeys.size());

        for (auto descriptorSetKey = descriptorSetKeys.begin(); descriptorSetKey != descriptorSetKeys.end();)
        {
            const auto descriptorSet = descriptorSetPool.Next();
            raster->setupDescriptorSets.emplace(*descriptorSetKey, descriptorSet);

            const auto imageAssetResource = descriptorSetKey->image->ResourceAs<Asset::Resource::Vulkan::Image>();
            const auto imageAssetDescriptor = imageAssetResource->descriptor;
            imageAssetDescriptor.Update(descriptorSet, *device);
            universalDataBuffer.Update(descriptorSet);

            descriptorSetKey = descriptorSetKeys.erase(descriptorSetKey);
        }

        raster->currentLayer = raster->layers.begin();
        return raster;
    }

    void QuadRenderer::MaterialCreated(Arca::Index<Asset::ImageMaterial> material)
    {
        mappedConduits.Add(material);
    }

    void QuadRenderer::MaterialDestroying(Arca::Index<Asset::ImageMaterial> material)
    {
        mappedConduits.Remove(material);
    }

    size_t QuadRenderer::RenderCount() const
    {
        return stagedImageRenders.size();
    }

    QuadRenderer::Quad::Quad(const Vertices& vertices) : vertices(vertices)
    {}

    QuadRenderer::Raster::Raster(
        vk::CommandBuffer commandBuffer,
        vk::CommandPool commandPool,
        QuadRenderer& renderer)
        :
        commandBuffer(commandBuffer),
        commandPool(commandPool),
        renderer(&renderer)
    {}

    void QuadRenderer::Raster::DrawNextLayer()
    {
        auto& layer = currentLayer->second;

        Draw(layer);

        ++currentLayer;
    }

    bool QuadRenderer::Raster::IsDone() const
    {
        return currentLayer == layers.end();
    }

    Spatial::Point3D::Value QuadRenderer::Raster::NextLayer() const
    {
        return currentLayer->first;
    }

    void QuadRenderer::Raster::Draw(Layer& layer)
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

    void QuadRenderer::Raster::WriteToBuffers(
        const Layer::MaterialGroup& materialGroup, MappedConduits::Group& conduitGroup)
    {
        for (auto& conduit : conduitGroup)
        {
            for (auto& value : materialGroup.values)
            {
                const auto descriptorSetKey = DescriptorSetKey(*value.first);
                const auto descriptorSet = setupDescriptorSets.find(descriptorSetKey)->second;

                conduit.Bind(commandBuffer);
                commandBuffer.bindDescriptorSets(
                    vk::PipelineBindPoint::eGraphics,
                    renderer->mappedConduits.PipelineLayout(),
                    0,
                    1,
                    &descriptorSet,
                    0,
                    nullptr);

                WriteToBuffers(value.second);
            }
        }
    }

    void QuadRenderer::Raster::WriteToBuffers(const std::vector<Quad>& quads)
    {
        const auto startQuadCount = quadCount;

        std::vector<Vertex> drawnVertices;
        std::vector<Index> drawnIndices;
        for (auto& quad : quads)
        {
            drawnVertices.insert(drawnVertices.end(), quad.vertices.begin(), quad.vertices.end());
            const auto offsetQuadIndicesBy = quadCount * indexIncrement;
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
            ++quadCount;
        }

        const auto vertexOffset = vk::DeviceSize(startQuadCount) * 4 * sizeof Vertex;
        const auto vertexSize = drawnVertices.size() * sizeof Vertex;
        const auto indexOffset = vk::DeviceSize(startQuadCount) * indices.size() * sizeof Index;
        const auto indexSize = drawnIndices.size() * sizeof Index;

        renderer->vertexBuffer.PushSourceBytes(drawnVertices.data(), vertexOffset, vertexSize);
        renderer->vertexBuffer.CopyFromSourceToDestination(vertexOffset, vertexSize, commandPool, renderer->graphicsQueue);
        renderer->indexBuffer.PushSourceBytes(drawnIndices.data(), indexOffset, indexSize);
        renderer->indexBuffer.CopyFromSourceToDestination(indexOffset, indexSize, commandPool, renderer->graphicsQueue);
        commandBuffer.drawIndexed(quads.size() * indices.size(), 1, startQuadCount * indices.size(), 0, 0);
    }

    void QuadRenderer::AddToRaster(const ImageRender& imageRender, Raster& raster)
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
        auto layer = raster.layers.Find(imageRender.position.z);
        if (!layer)
            layer = &raster.layers.Add(imageRender.position.z, Raster::Layer{});
        auto& group = layer->GroupFor(imageRender.materialID);
        group.ListFor(imageAsset).emplace_back(vertices);
        descriptorSetKeys.emplace(DescriptorSetKey(*imageAsset));
    }
}