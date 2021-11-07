#include "VulkanQuadRenderer.h"

#include "VulkanImageAssetResource.h"
#include "VulkanTextResource.h"
#include "VulkanCommandBuffer.h"
#include "VulkanUtilities.h"

#include "SpatialAlgorithms.h"

namespace Atmos::Render::Vulkan
{
    QuadRendererDescriptorSetKey::QuadRendererDescriptorSetKey(const CombinedImageSamplerDescriptor& descriptor) :
        descriptor(&descriptor)
    {}

    bool QuadRendererDescriptorSetKey::operator==(const QuadRendererDescriptorSetKey& arg) const
    {
        return descriptor == arg.descriptor;
    }

    QuadRenderer::QuadRenderer(
        vk::Device device,
        vk::Queue graphicsQueue,
        vk::PhysicalDeviceMemoryProperties memoryProperties,
        vk::RenderPass renderPass,
        vk::Extent2D swapchainExtent)
        :
        memoryPool(0, memoryProperties, device),
        vertexBuffer(vertexStride * sizeof(Vertex), device, memoryPool, vk::BufferUsageFlagBits::eVertexBuffer),
        indexBuffer(indexStride * sizeof(Index), device, memoryPool, vk::BufferUsageFlagBits::eIndexBuffer),
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
            {},
            { descriptorSetPool.DescriptorSetLayout() }),
        graphicsQueue(graphicsQueue),
        device(device)
    {}

    void QuadRenderer::StageRender(const RenderImage& imageRender)
    {
        stagedImageRenders.push_back(imageRender);
    }

    void QuadRenderer::StageRender(const RenderText& textRender)
    {
        stagedTextRenders.push_back(textRender);
    }
    
    std::unique_ptr<Raster> QuadRenderer::Start(
        vk::CommandBuffer drawCommandBuffer,
        const UniversalDataBuffer& universalDataBuffer)
    {
        const auto totalQuadCount = stagedImageRenders.size() + stagedTextRenders.size();
        if (totalQuadCount == 0)
            return {};
        
        auto raster = std::make_unique<Raster>(*this);

        for (auto& stagedImageRender : stagedImageRenders)
        {
            mappedConduits.Add(stagedImageRender.material);
            AddToRaster(stagedImageRender, *raster);
        }
        stagedImageRenders.clear();

        for (auto& stagedTextRender : stagedTextRenders)
        {
            mappedConduits.Add(stagedTextRender.material);
            AddToRaster(stagedTextRender, *raster);
        }
        stagedTextRenders.clear();

        descriptorSetPool.Reset();
        descriptorSetPool.Reserve(descriptorSetKeys.size());

        for (auto& descriptorSetKey : descriptorSetKeys)
        {
            const auto descriptorSet = descriptorSetPool.Next();
            raster->descriptorSets.emplace(descriptorSetKey, descriptorSet);
            
            const auto& descriptor = *descriptorSetKey.descriptor;
            descriptor.Update(descriptorSet, device);
            universalDataBuffer.Update(descriptorSet);
        }
        descriptorSetKeys.clear();

        raster->currentLayer = raster->layers.begin();
        return raster;
    }
    
    void QuadRenderer::MaterialDestroying(Arca::Index<Asset::Material> material)
    {
        mappedConduits.Remove(material);
    }

    size_t QuadRenderer::RenderCount() const
    {
        return stagedImageRenders.size();
    }

    QuadRenderer::Quad::Quad(const Vertices& vertices) : vertices(vertices)
    {}

    QuadRenderer::Raster::Raster(QuadRenderer& renderer) : renderer(&renderer)
    {}

    auto QuadRenderer::Raster::NextPasses() -> std::vector<Pass>
    {
        const auto& layer = currentLayer->second;
        auto passes = NextPasses(layer);
        ++currentLayer;
        return passes;
    }

    bool QuadRenderer::Raster::IsDone() const
    {
        return currentLayer == layers.end();
    }

    ObjectLayeringKey QuadRenderer::Raster::NextLayer() const
    {
        return currentLayer->first;
    }
    
    auto QuadRenderer::Raster::NextPasses(const Layer& layer) -> std::vector<Pass>
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

    auto QuadRenderer::Raster::NextPasses(const Layer::MaterialGroup& materialGroup, MappedConduits::Group& conduitGroup) -> std::vector<Pass>
    {
        std::vector<Pass> passes;
        for (auto& conduit : conduitGroup)
        {
            for (auto& value : materialGroup.values)
            {
                const auto descriptorSet = descriptorSets.find(DescriptorSetKey(*value.first))->second;
                
                const auto startQuadCount = totalQuadCount;
                const auto quads = value.second;
                passes.emplace_back(
                    WriteData(quads, startQuadCount),
                    Draw(startQuadCount, quads.size(), conduit, descriptorSet));
                totalQuadCount += quads.size();
            }
        }
        return passes;
    }

    Command QuadRenderer::Raster::WriteData(const std::vector<Quad>& quads, std::uint32_t startQuadCount)
    {
        std::vector<Vertex> verticesToDraw;
        std::vector<Index> indicesToDraw;
        std::uint32_t currentQuad = 0;
        for (auto& quad : quads)
        {
            verticesToDraw.insert(verticesToDraw.end(), quad.vertices.begin(), quad.vertices.end());
            const auto offsetIndicesBy = (startQuadCount + currentQuad) * indexIncrement;
            const Indices offsetIndices =
            {
                Index(offsetIndicesBy + indices[0]),
                Index(offsetIndicesBy + indices[1]),
                Index(offsetIndicesBy + indices[2]),
                Index(offsetIndicesBy + indices[3]),
                Index(offsetIndicesBy + indices[4]),
                Index(offsetIndicesBy + indices[5]),
            };
            indicesToDraw.insert(indicesToDraw.end(), offsetIndices.begin(), offsetIndices.end());
            ++currentQuad;
        }
        
        const auto vertexOffset = vk::DeviceSize(startQuadCount) * 4 * sizeof Vertex;
        const auto vertexSize = verticesToDraw.size() * sizeof Vertex;
        const auto indexOffset = vk::DeviceSize(startQuadCount) * indices.size() * sizeof Index;
        const auto indexSize = indicesToDraw.size() * sizeof Index;

        renderer->vertexBuffer.PushSourceBytes(verticesToDraw.data(), vertexOffset, vertexSize);
        renderer->indexBuffer.PushSourceBytes(indicesToDraw.data(), indexOffset, indexSize);

        return [this, vertexOffset, vertexSize, indexOffset, indexSize](CommandRecorder record)
        {
            record(renderer->vertexBuffer.CopyFromSourceToDestination(vertexOffset, vertexSize));
            record(renderer->indexBuffer.CopyFromSourceToDestination(indexOffset, indexSize));
        };
    }

	Command QuadRenderer::Raster::Draw(std::uint32_t startQuadCount, std::uint32_t quadCount, Conduit& conduit, vk::DescriptorSet descriptorSet)
    {
        return [this, startQuadCount, quadCount, &conduit, descriptorSet](CommandRecorder record)
        {
            record(conduit.Bind());
            record([this, startQuadCount, quadCount, &conduit, descriptorSet](vk::CommandBuffer commandBuffer)
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
                        &descriptorSet,
                        0,
                        nullptr);

                    commandBuffer.drawIndexed(quadCount * indices.size(), 1, startQuadCount * indices.size(), 0, 0);
                });
        };
    }

    void QuadRenderer::AddToRaster(const RenderImage& imageRender, Raster& raster)
    {
        const auto assetResource = dynamic_cast<Asset::Resource::Vulkan::Image*>(imageRender.assetResource);
        
        const auto vertices = ToVertices(
            imageRender.color,
            ToPoint2D(imageRender.position),
            imageRender.size,
            imageRender.rotation,
            ToTextureSlice(imageRender.slice, assetResource->imageData.size));
        AddToRaster(
            imageRender.space,
            imageRender.position.z,
            imageRender.material.ID(),
            assetResource->imageData.descriptor,
            vertices,
            raster);
    }

    void QuadRenderer::AddToRaster(const RenderText& textRender, Raster& raster)
    {
        const auto textResource = dynamic_cast<Resource::Vulkan::Text*>(textRender.resource);

        const auto vertices = ToVertices(
            textRender.color,
            ToPoint2D(textRender.position),
            textRender.size,
            textRender.rotation,
            ToTextureSlice(textRender.slice, textResource->imageData.size));
        AddToRaster(
            textRender.space,
            textRender.position.z,
            textRender.material.ID(),
            textResource->imageData.descriptor,
            vertices,
            raster);
    }

    void QuadRenderer::AddToRaster(
        int space,
        Spatial::Point3D::Value z,
        Arca::RelicID materialID,
        CombinedImageSamplerDescriptor& descriptor,
        std::array<Vertex, 4> vertices,
        Raster& raster)
    {
        const auto layerKey = ObjectLayeringKey{ space, z };
        auto layer = raster.layers.Find(layerKey);
        if (!layer)
            layer = &raster.layers.Add(layerKey, Raster::Layer{});
        auto& group = layer->GroupFor(materialID);
        group.ListFor(&descriptor).emplace_back(vertices);
        descriptorSetKeys.emplace(DescriptorSetKey(descriptor));
    }

    auto QuadRenderer::ToVertices(
        Color color,
        Spatial::Point2D position,
        Spatial::Size2D size,
        Spatial::Angle2D rotation,
        Spatial::AxisAlignedBox2D texture)

        -> std::array<Vertex, 4>
    {
        constexpr auto rotate = [](
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
        
        const auto halfWidth = size.width / 2;
        const auto halfHeight = size.height / 2;
        const auto topLeft = rotate({ -halfWidth, -halfHeight }, rotation, position);
        const auto topRight = rotate({ halfWidth, -halfHeight }, rotation, position);
        const auto bottomLeft = rotate({ -halfWidth, halfHeight }, rotation, position);
        const auto bottomRight = rotate({ halfWidth, halfHeight }, rotation, position);

        const auto useColor = AtmosToVulkanColor(color);

        return std::array
        {
            Vertex
            {
                useColor,
                { topLeft.x, topLeft.y },
                { texture.Left(), texture.Top() }
            },
            Vertex
            {
                useColor,
                { topRight.x, topRight.y },
                { texture.Right(), texture.Top() }
            },
            Vertex
            {
                useColor,
                { bottomLeft.x, bottomLeft.y },
                { texture.Left(), texture.Bottom() }
            },
            Vertex
            {
                useColor,
                { bottomRight.x, bottomRight.y },
                { texture.Right(), texture.Bottom() }
            }
        };
    }

    Spatial::AxisAlignedBox2D QuadRenderer::ToTextureSlice(
        const Spatial::AxisAlignedBox2D& inputSlice, const Spatial::Size2D& resourceSize)
    {
        constexpr auto clamp = [](float input)
        {
            return std::clamp(input, 0.0f, 1.0f);
        };

        return Spatial::ToAxisAlignedBox2D(
            clamp(inputSlice.Left() / resourceSize.width),
            clamp(inputSlice.Top() / resourceSize.height),
            clamp(inputSlice.Right() / resourceSize.width),
            clamp(inputSlice.Bottom() / resourceSize.height));
    }
}