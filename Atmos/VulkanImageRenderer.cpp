#include "VulkanImageRenderer.h"

#include "VulkanImageAssetResource.h"
#include "VulkanCommandBuffer.h"
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
        vertexBuffer(vertexStride * sizeof(TexturedVertex), device, memoryPool, vk::BufferUsageFlagBits::eVertexBuffer),
        indexBuffer(indexStride * sizeof(TexturedIndex), device, memoryPool, vk::BufferUsageFlagBits::eIndexBuffer),
        descriptorSetPools(CreateDescriptorSetPools(device)),
        currentDescriptorSetPool(descriptorSetPools.begin()),
        mappedConduits(
            device,
            VertexInput(
                sizeof(TexturedVertex),
                {
                    vk::VertexInputAttributeDescription(
                        0, 0, vk::Format::eR32G32Sfloat, offsetof(TexturedVertex, position)),
                    vk::VertexInputAttributeDescription(
                        1, 0, vk::Format::eR32G32Sfloat, offsetof(TexturedVertex, texture)),
                    vk::VertexInputAttributeDescription(
                        2, 0, vk::Format::eR32G32B32A32Sfloat, offsetof(TexturedVertex, color))
                }),
            vk::PrimitiveTopology::eTriangleList,
            renderPass,
            swapchainExtent,
            {},
            { descriptorSetPools[0].DescriptorSetLayout()}),
        graphicsQueue(graphicsQueue),
        device(device)
    {}
    
    std::unique_ptr<Raster> ImageRenderer::Start(
        const AllRenders& allRenders,
        vk::CommandBuffer drawCommandBuffer,
        const UniversalDataBuffer& universalDataBuffer)
    {
        auto raster = std::make_unique<Raster>(*this);

        for (auto& render : allRenders.images)
        {
            mappedConduits.Add(*render.material);
            AddToRaster(render, *raster);
        }

        if (!raster->layers.Empty())
        {
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
        else
            return {};
    }
    
    void ImageRenderer::MaterialDestroying(const Asset::Material& material)
    {
        mappedConduits.Remove(material);
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
            const auto conduitGroup = renderer->mappedConduits.For(*materialGroup.first);
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

                const auto elements = value.second;
                const auto startVertexCount = totalVertexCount;
                const auto startIndexCount = totalIndexCount;

                std::uint32_t vertexCount = 0;
                std::uint32_t indexCount = 0;
                for (auto& element : elements)
                {
                    vertexCount += element.vertices.size();
                    indexCount += element.indices.size();
                }

                passes.emplace_back(
                    WriteData(elements, startVertexCount, startIndexCount),
                    Draw(vertexCount, indexCount, startIndexCount, conduit, descriptorSet));
                for (auto& element : elements)
                {
                    totalVertexCount += element.vertices.size();
                    totalIndexCount += element.indices.size();
                }
            }
        }
        return passes;
    }

    Command ImageRenderer::Raster::WriteData(
        const std::vector<Textured>& elements, std::uint32_t startVertexCount, std::uint32_t startIndexCount)
    {
        std::vector<TexturedVertex> verticesToDraw;
        std::vector<TexturedIndex> indicesToDraw;
        auto vertexCount = startVertexCount;
        for (auto& element : elements)
        {
            verticesToDraw.insert(verticesToDraw.end(), element.vertices.begin(), element.vertices.end());
            for(auto& index : element.indices)
                indicesToDraw.push_back(vertexCount + index);
            vertexCount += element.vertices.size();
        }

        const auto vertexOffset = vk::DeviceSize(startVertexCount) * sizeof TexturedVertex;
        const auto vertexSize = verticesToDraw.size() * sizeof TexturedVertex;
        const auto indexOffset = vk::DeviceSize(startIndexCount) * sizeof TexturedIndex;
        const auto indexSize = indicesToDraw.size() * sizeof TexturedIndex;

        renderer->vertexBuffer.PushSourceBytes(verticesToDraw.data(), vertexOffset, vertexSize);
        renderer->indexBuffer.PushSourceBytes(indicesToDraw.data(), indexOffset, indexSize);

        return [this, vertexOffset, vertexSize, indexOffset, indexSize](CommandRecorder record)
        {
            record(renderer->vertexBuffer.CopyFromSourceToDestination(vertexOffset, vertexSize));
            record(renderer->indexBuffer.CopyFromSourceToDestination(indexOffset, indexSize));
        };
    }

	Command ImageRenderer::Raster::Draw(
        std::uint32_t vertexCount, std::uint32_t indexCount, std::uint32_t startIndexCount, Conduit& conduit, vk::DescriptorSet descriptorSet)
    {
        return [this, vertexCount, indexCount, startIndexCount, &conduit, descriptorSet](CommandRecorder record)
        {
            record(conduit.Bind());
            record([this, vertexCount, indexCount, startIndexCount, &conduit, descriptorSet](vk::CommandBuffer commandBuffer)
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

                    commandBuffer.drawIndexed(indexCount, 1, startIndexCount, 0, 0);
                });
        };
    }

    void ImageRenderer::AddToRaster(const RenderImage& imageRender, Raster& raster)
    {
        const auto assetResource = dynamic_cast<Asset::Resource::Vulkan::Image*>(imageRender.assetResource);
        const auto position = ToPoint2D(imageRender.position);
        const auto clipTo = Spatial::ToPoints(imageRender.viewSlice, 0, Spatial::Point2D{});

        const auto worldTopLeft = position
            - Spatial::Point2D{ (imageRender.size.width / 2) * imageRender.scalers.x, (imageRender.size.height / 2) * imageRender.scalers.y };
        const auto textured = ToTextured(
            imageRender.color,
            worldTopLeft,
            Spatial::Point2D{ imageRender.assetSlice.Left(), imageRender.assetSlice.Top() },
            imageRender.size,
            assetResource->Size(),
            imageRender.scalers,
            imageRender.rotation,
            Spatial::Point2D{ 0.5, 0.5 },
            clipTo);
        if (textured)
            AddToRaster(
                imageRender.space,
                imageRender.position.z,
                *imageRender.material,
                assetResource->imageData.descriptor,
                *textured,
                raster);
    }

    void ImageRenderer::AddToRaster(
        int space,
        Spatial::Point3D::Value z,
        const Asset::Material& material,
        const CombinedImageSamplerDescriptor& descriptor,
        Textured element,
        Raster& raster)
    {
        const auto layerKey = ObjectLayeringKey{ space, z };
        auto layer = raster.layers.Find(layerKey);
        if (!layer)
            layer = &raster.layers.Add(layerKey, Raster::Layer{});
        auto& group = layer->GroupFor(material);
        group.ListFor(&descriptor).emplace_back(element);
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