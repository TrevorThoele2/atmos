#include "VulkanImageRenderer.h"

#include "VulkanImageAssetResource.h"
#include "VulkanCommandBuffer.h"
#include "VulkanUtilities.h"
#include "VulkanMaxFramesInFlight.h"

#include "SpatialAlgorithms.h"

namespace Atmos::Render::Vulkan
{
    ImageRendererDescriptorSetKey::ImageRendererDescriptorSetKey(const CombinedImageSamplerDescriptor& descriptor) :
        descriptor(&descriptor)
    {}

    bool ImageRendererDescriptorSetKey::operator==(const ImageRendererDescriptorSetKey& arg) const
    {
        return descriptor == arg.descriptor;
    }

    ImageRenderer::ImageRenderer(
        vk::Device device,
        vk::Queue graphicsQueue,
        vk::PhysicalDeviceMemoryProperties memoryProperties,
        vk::RenderPass renderPass,
        vk::Extent2D swapchainExtent)
        :
        memoryPool(0, memoryProperties, device),
        vertexBuffer(vertexStride * sizeof(QuadVertex), device, memoryPool, vk::BufferUsageFlagBits::eVertexBuffer),
        indexBuffer(indexStride * sizeof(QuadIndex), device, memoryPool, vk::BufferUsageFlagBits::eIndexBuffer),
        descriptorSetPools(CreateDescriptorSetPools(device)),
        currentDescriptorSetPool(descriptorSetPools.begin()),
        mappedConduits(
            device,
            VertexInput(
                sizeof(QuadVertex),
                {
                    vk::VertexInputAttributeDescription(
                        0, 0, vk::Format::eR32G32Sfloat, offsetof(QuadVertex, position)),
                    vk::VertexInputAttributeDescription(
                        1, 0, vk::Format::eR32G32Sfloat, offsetof(QuadVertex, texture)),
                    vk::VertexInputAttributeDescription(
                        2, 0, vk::Format::eR32G32B32A32Sfloat, offsetof(QuadVertex, color))
                }),
            vk::PrimitiveTopology::eTriangleList,
            renderPass,
            swapchainExtent,
            {},
            { descriptorSetPools[0].DescriptorSetLayout()}),
        graphicsQueue(graphicsQueue),
        device(device)
    {}

    void ImageRenderer::StageRender(const RenderImage& imageRender)
    {
        stagedImageRenders.push_back(imageRender);
    }

    std::unique_ptr<Raster> ImageRenderer::Start(
        vk::CommandBuffer drawCommandBuffer,
        const UniversalDataBuffer& universalDataBuffer)
    {
        const auto totalQuadCount = stagedImageRenders.size();
        if (totalQuadCount == 0)
            return {};
        
        auto raster = std::make_unique<Raster>(*this);

        for (auto& stagedImageRender : stagedImageRenders)
        {
            mappedConduits.Add(stagedImageRender.material);
            AddToRaster(stagedImageRender, *raster);
        }
        stagedImageRenders.clear();

        auto& descriptorSetPool = NextDescriptorSetPool();
        const auto descriptorSets = descriptorSetPool.Retrieve(descriptorSetKeys.size());

        size_t i = 0;
        for (auto& descriptorSetKey : descriptorSetKeys)
        {
            const auto descriptorSet = descriptorSets[i];
            raster->descriptorSets.emplace(descriptorSetKey, descriptorSet);
            
            const auto& descriptor = *descriptorSetKey.descriptor;
            descriptor.Update(descriptorSet, device);
            universalDataBuffer.Update(descriptorSet);

            ++i;
        }
        descriptorSetKeys.clear();

        raster->currentLayer = raster->layers.begin();
        return raster;
    }
    
    void ImageRenderer::MaterialDestroying(Arca::Index<Asset::Material> material)
    {
        mappedConduits.Remove(material);
    }

    size_t ImageRenderer::RenderCount() const
    {
        return stagedImageRenders.size();
    }

    ImageRenderer::Raster::Raster(ImageRenderer& renderer) : renderer(&renderer)
    {}

    auto ImageRenderer::Raster::NextPasses() -> std::vector<Pass>
    {
        const auto& layer = currentLayer->second;
        auto passes = NextPasses(layer);
        ++currentLayer;
        return passes;
    }

    bool ImageRenderer::Raster::IsDone() const
    {
        return currentLayer == layers.end();
    }

    ObjectLayeringKey ImageRenderer::Raster::NextLayer() const
    {
        return currentLayer->first;
    }
    
    auto ImageRenderer::Raster::NextPasses(const Layer& layer) -> std::vector<Pass>
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

    auto ImageRenderer::Raster::NextPasses(const Layer::MaterialGroup& materialGroup, MappedConduits::Group& conduitGroup) -> std::vector<Pass>
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

    Command ImageRenderer::Raster::WriteData(const std::vector<Quad>& quads, std::uint32_t startQuadCount)
    {
        std::vector<QuadVertex> verticesToDraw;
        std::vector<QuadIndex> indicesToDraw;
        std::uint32_t currentQuad = 0;
        for (auto& quad : quads)
        {
            verticesToDraw.insert(verticesToDraw.end(), quad.vertices.begin(), quad.vertices.end());
            const auto offsetIndicesBy = (startQuadCount + currentQuad) * quadIndexIncrement;
            const QuadIndices offsetIndices =
            {
                QuadIndex(offsetIndicesBy + quadIndices[0]),
                QuadIndex(offsetIndicesBy + quadIndices[1]),
                QuadIndex(offsetIndicesBy + quadIndices[2]),
                QuadIndex(offsetIndicesBy + quadIndices[3]),
                QuadIndex(offsetIndicesBy + quadIndices[4]),
                QuadIndex(offsetIndicesBy + quadIndices[5]),
            };
            indicesToDraw.insert(indicesToDraw.end(), offsetIndices.begin(), offsetIndices.end());
            ++currentQuad;
        }
        
        const auto vertexOffset = vk::DeviceSize(startQuadCount) * 4 * sizeof QuadVertex;
        const auto vertexSize = verticesToDraw.size() * sizeof QuadVertex;
        const auto indexOffset = vk::DeviceSize(startQuadCount) * quadIndices.size() * sizeof QuadIndex;
        const auto indexSize = indicesToDraw.size() * sizeof QuadIndex;

        renderer->vertexBuffer.PushSourceBytes(verticesToDraw.data(), vertexOffset, vertexSize);
        renderer->indexBuffer.PushSourceBytes(indicesToDraw.data(), indexOffset, indexSize);

        return [this, vertexOffset, vertexSize, indexOffset, indexSize](CommandRecorder record)
        {
            record(renderer->vertexBuffer.CopyFromSourceToDestination(vertexOffset, vertexSize));
            record(renderer->indexBuffer.CopyFromSourceToDestination(indexOffset, indexSize));
        };
    }

	Command ImageRenderer::Raster::Draw(std::uint32_t startQuadCount, std::uint32_t quadCount, Conduit& conduit, vk::DescriptorSet descriptorSet)
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

                    commandBuffer.drawIndexed(quadCount * quadIndices.size(), 1, startQuadCount * quadIndices.size(), 0, 0);
                });
        };
    }

    void ImageRenderer::AddToRaster(const RenderImage& imageRender, Raster& raster)
    {
        const auto assetResource = dynamic_cast<Asset::Resource::Vulkan::Image*>(imageRender.assetResource);
        const auto position = ToPoint2D(imageRender.position);
        const auto imageSize = assetResource->imageData.size;
        const auto slice = Spatial::Clamp(imageRender.slice, Spatial::ToAxisAlignedBox2D(0, 0, 1, 1));
        const auto size = Spatial::Size2D
        {
            slice.size.width * imageSize.width * imageRender.scalers.x,
            slice.size.height * imageSize.height * imageRender.scalers.y
        };

        const auto vertices = ToQuadVertices(
            imageRender.color,
            position,
            size,
            imageRender.rotation,
            position,
            slice);
        AddToRaster(
            imageRender.space,
            imageRender.position.z,
            imageRender.material.ID(),
            assetResource->imageData.descriptor,
            vertices,
            raster);
    }

    void ImageRenderer::AddToRaster(
        int space,
        Spatial::Point3D::Value z,
        Arca::RelicID materialID,
        CombinedImageSamplerDescriptor& descriptor,
        std::array<QuadVertex, 4> vertices,
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
    
    std::vector<DescriptorSetPool> ImageRenderer::CreateDescriptorSetPools(vk::Device device)
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
                    },
                        DescriptorSetPool::Definition
                    {
                        vk::DescriptorType::eCombinedImageSampler,
                        1,
                        vk::ShaderStageFlagBits::eFragment,
                        1
                    }
                },
                device);
        }
        return pools;
    }

    DescriptorSetPool& ImageRenderer::NextDescriptorSetPool()
    {
        ++currentDescriptorSetPool;
        if (currentDescriptorSetPool == descriptorSetPools.end())
            currentDescriptorSetPool = descriptorSetPools.begin();
        return *currentDescriptorSetPool;
    }
}