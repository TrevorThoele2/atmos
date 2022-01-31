#include "VulkanTextRenderer.h"

#include "VulkanImage.h"
#include "VulkanSampler.h"
#include "VulkanCommandBuffer.h"
#include "VulkanUtilities.h"
#include "VulkanSynchronization.h"
#include "VulkanMaxFramesInFlight.h"
#include "GlyphAlgorithms.h"

#include "SpatialAlgorithms.h"

#include <utf8.h>

namespace Atmos::Render::Vulkan
{
    TextRendererDescriptorSetKey::TextRendererDescriptorSetKey(const CombinedImageSamplerDescriptor& descriptor) :
        descriptor(&descriptor)
    {}

    bool TextRendererDescriptorSetKey::operator==(const TextRendererDescriptorSetKey& arg) const
    {
        return descriptor == arg.descriptor;
    }

    TextRenderer::TextRenderer(
        vk::Device device,
        vk::Queue graphicsQueue,
        vk::PhysicalDeviceMemoryProperties memoryProperties,
        vk::RenderPass renderPass,
        vk::Extent2D swapchainExtent,
        GlyphAtlas& glyphAtlas)
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
        device(device),
        glyphAtlas(&glyphAtlas),
        sampler(CreateSampler(device))
    {}

    void TextRenderer::StageRender(const RenderText& textRender)
    {
        const auto stagedRender = ToStagedRender(textRender);
        if (stagedRender)
            stagedTextRenders.push_back(*stagedRender);
    }

    std::unique_ptr<Raster> TextRenderer::Start(
        vk::CommandBuffer drawCommandBuffer,
        const UniversalDataBuffer& universalDataBuffer)
    {
        const auto totalQuadCount = stagedTextRenders.size();
        if (totalQuadCount == 0)
            return {};

        descriptors.clear();

        auto raster = std::make_unique<Raster>(*this);

        for (auto& stagedTextRender : stagedTextRenders)
        {
            mappedConduits.Add(stagedTextRender.material);
            AddToRaster(stagedTextRender, *raster);
        }
        stagedTextRenders.clear();

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

    void TextRenderer::MaterialDestroying(Arca::Index<Asset::Material> material)
    {
        mappedConduits.Remove(material);
    }

    size_t TextRenderer::RenderCount() const
    {
        return stagedTextRenders.size();
    }

    auto TextRenderer::ToStagedRender(const RenderText& render) -> std::optional<StagedTextRender>
    {
        const auto result = glyphAtlas->ToLines(
            { render.fontResource, render.bold, render.italics }, render.string, render.wrapWidth);
        if (!result)
            return {};

        return StagedTextRender
        {
            result->glyphs,
            result->totalSize,
            result->imageSize,
            result->cellSize,
            result->imageView,
            render.fontResource,
            render.slice,
            render.material,
            render.position,
            render.rotation,
            render.scalers,
            render.color,
            render.bold,
            render.italics,
            render.wrapWidth,
            render.space,
            render.surface
        };
    }

    TextRenderer::Raster::Raster(TextRenderer& renderer) : renderer(&renderer)
    {}

    auto TextRenderer::Raster::NextPasses() -> std::vector<Pass>
    {
        const auto& layer = currentLayer->second;
        auto passes = NextPasses(layer);
        ++currentLayer;
        return passes;
    }

    bool TextRenderer::Raster::IsDone() const
    {
        return currentLayer == layers.end();
    }

    ObjectLayeringKey TextRenderer::Raster::NextLayer() const
    {
        return currentLayer->first;
    }

    auto TextRenderer::Raster::NextPasses(const Layer& layer) -> std::vector<Pass>
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

    auto TextRenderer::Raster::NextPasses(const Layer::MaterialGroup& materialGroup, MappedConduits::Group& conduitGroup) -> std::vector<Pass>
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

    Command TextRenderer::Raster::WriteData(const std::vector<Quad>& quads, std::uint32_t startQuadCount)
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

    Command TextRenderer::Raster::Draw(
        std::uint32_t startQuadCount, std::uint32_t quadCount, Conduit& conduit, vk::DescriptorSet descriptorSet)
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

    void TextRenderer::AddToRaster(const StagedTextRender& render, Raster& raster)
    {
        const auto fontID = FontID{ render.fontResource, render.bold, render.italics };
        const auto descriptor = &descriptors.emplace(
            fontID,
            CombinedImageSamplerDescriptor(
                render.atlasImageView,
                sampler.get(),
                vk::ImageLayout::eGeneral,
                1)).first->second;

        const auto vertices = ToQuads(render);
        AddToRaster(
            render.space,
            render.position.z,
            render.material.ID(),
            *descriptor,
            vertices,
            raster);
    }

    void TextRenderer::AddToRaster(
        int space,
        Spatial::Point3D::Value z,
        Arca::RelicID materialID,
        Vulkan::CombinedImageSamplerDescriptor& descriptor,
        std::vector<Quad> quads,
        Raster& raster)
    {
        const auto layerKey = ObjectLayeringKey{ space, z };
        auto layer = raster.layers.Find(layerKey);
        if (!layer)
            layer = &raster.layers.Add(layerKey, Raster::Layer{});
        auto& group = layer->GroupFor(materialID);
        auto& list = group.ListFor(&descriptor);
        list.insert(list.end(), quads.begin(), quads.end());
        descriptorSetKeys.emplace(DescriptorSetKey(descriptor));
    }

    std::vector<Quad> TextRenderer::ToQuads(const StagedTextRender& render)
    {
        std::vector<Quad> quads;

        const auto scalers = render.scalers;
        const auto position = Spatial::ToPoint2D(render.position);
        const auto atlasImageSize = render.atlasImageSize;
        const auto atlasCellSize = render.atlasCellSize;
        const auto totalSize = Spatial::ScaleBy(render.totalSize, render.scalers);

        const auto topLeft = position - Spatial::Point2D{ totalSize.width / 2, totalSize.height / 2 };

        const auto slice = Spatial::Clamp(render.slice, Spatial::ToAxisAlignedBox2D(0, 0, 1, 1));
        const auto maximumSlice = Spatial::ToAxisAlignedBox2D(
            slice.Left() * totalSize.width,
            slice.Top() * totalSize.height,
            slice.Right() * totalSize.width,
            slice.Bottom() * totalSize.height);

        auto x = 0.0f;
        auto y = 0.0f;
        for (auto& line : render.lines)
        {
            for (auto& glyph : line)
            {
                const auto width = glyph.size.width;
                const auto height = glyph.size.height;
                const auto advance = glyph.advance;

                const auto glyphStandard = Spatial::ToAxisAlignedBox2D(x, y, x + width * scalers.x, y + height * scalers.y);
                const auto glyphSlice = Spatial::Clamp(glyphStandard, maximumSlice);
                const auto glyphPercentage = Spatial::ScaleOf(glyphSlice, glyphStandard);

                if (glyphSlice.size.width > 0.0f && glyphSlice.size.height > 0.0f)
                {
                    const auto texture = Spatial::ScaleOf(
                        Spatial::ToAxisAlignedBox2D(
                            glyph.column * atlasCellSize.width + width * glyphPercentage.Left(),
                            glyph.row * atlasCellSize.height + height * glyphPercentage.Top(),
                            glyph.column * atlasCellSize.width + width * glyphPercentage.Right(),
                            glyph.row * atlasCellSize.height + height * glyphPercentage.Bottom()),
                        Spatial::ToAxisAlignedBox2D(0, 0, atlasImageSize.width, atlasImageSize.height));

                    const auto center = glyphSlice.center + topLeft;
                    quads.push_back(ToQuadVertices(
                        render.color,
                        center,
                        glyphSlice.size,
                        render.rotation,
                        position,
                        texture));
                }

                x += advance * scalers.x;
            }

            x = 0;
            y += atlasCellSize.height * scalers.y;
        }

        return quads;
    }

    std::vector<DescriptorSetPool> TextRenderer::CreateDescriptorSetPools(vk::Device device)
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

    DescriptorSetPool& TextRenderer::NextDescriptorSetPool()
    {
        ++currentDescriptorSetPool;
        if (currentDescriptorSetPool == descriptorSetPools.end())
            currentDescriptorSetPool = descriptorSetPools.begin();
        return *currentDescriptorSetPool;
    }
}