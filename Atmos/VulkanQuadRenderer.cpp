#include "VulkanQuadRenderer.h"

#include "VulkanImageAssetResource.h"
#include "VulkanCommandBuffer.h"
#include "VulkanMaxFramesInFlight.h"

#include "RasterCommand.h"
#include "SpatialAlgorithms.h"

namespace Atmos::Render::Vulkan
{
    QuadRenderer::QuadRenderer(
        vk::Device device,
        vk::Queue graphicsQueue,
        vk::PhysicalDeviceMemoryProperties memoryProperties,
        vk::RenderPass renderPass,
        vk::Extent2D swapchainExtent,
        GlyphAtlas& glyphAtlas)
        :
        memoryPool(0, memoryProperties, device),
        vertexBuffer(vertexStride * sizeof(QuadVertex), device, memoryPool, vk::BufferUsageFlagBits::eVertexBuffer),
        indexBuffer(indexStride * sizeof(QuadVertex), device, memoryPool, vk::BufferUsageFlagBits::eIndexBuffer),
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
        glyphAtlas(&glyphAtlas),
        graphicsQueue(graphicsQueue),
        device(device)
    {}

    void QuadRenderer::Start()
    {
        totalElements = 0;
    }
    
    std::optional<RendererPass> QuadRenderer::Draw(
        const std::vector<Raster::DrawImage>& draws, const UniversalDataBuffer& universalDataBuffer)
    {
        if (std::find_if(draws.begin(), draws.end(), [](const Raster::Command& arg) { return std::holds_alternative<Raster::DrawImage>(arg) || std::holds_alternative<Raster::DrawText>(arg); }) == draws.end())
            return {};
        
        struct Group
        {
            const Asset::Resource::Image* assetResource = nullptr;
            Shaders shaders;
            std::vector<Quad> elements = {};
        };

        std::vector<Group> groups;
        auto currentGroup = &groups.emplace_back(
            draws.begin()->asset->Resource(),
            draws.begin()->shaders,
            std::vector<Quad>{});
        for (auto& draw : draws)
        {
            if (draw.asset->Resource() != currentGroup->assetResource || draw.shaders != currentGroup->shaders)
            {
                currentGroup = &groups.emplace_back(
                    draw.asset->Resource(),
                    draw.shaders,
                    std::vector<Quad>{});
            }
            
            const auto quad = ToQuad(
                draw.color,
                draw.position,
                draw.size,
                draw.rotation,
                draw.position,
                Clamp(draw.assetSlice, Spatial::ToAxisAlignedBox2D(0, 0, 1, 1)));

            currentGroup->elements.push_back(quad);
        }

        auto& descriptorSetPool = NextDescriptorSetPool();
        const auto descriptorSets = descriptorSetPool.Retrieve(groups.size());
        
        return RendererPass
        {
            Command
            {
                [this, groups, descriptorSets, &universalDataBuffer](CommandRecorder record)
                {
                    auto drawnElements = 0;

                    size_t i = 0;
                    for (auto& group : groups)
                    {
                        const auto descriptorSet = descriptorSets[i];

                        auto& descriptor = dynamic_cast<const Asset::Resource::Vulkan::Image*>(group.assetResource)->imageData.descriptor;
                        descriptor.Update(descriptorSet, device);
                        universalDataBuffer.Update(descriptorSet);

                        record(WriteData(group.elements, totalElements + drawnElements));

                        drawnElements += group.elements.size();
                        ++i;
                    }
                }
            },
            Command
            {
                [this, groups, descriptorSets](CommandRecorder record)
                {
                    auto drawnElements = 0;

                    size_t i = 0;
                    for (auto& group : groups)
                    {
                        const auto descriptorSet = descriptorSets[i];
                        
                        record(Draw(
                            group.elements.size() * verticesPerQuad,
                            group.elements.size() * indicesPerQuad,
                            (totalElements + drawnElements) * indicesPerQuad,
                            mappedConduits.For(group.shaders),
                            descriptorSet));

                        drawnElements += group.elements.size();
                        ++i;
                    }

                    totalElements += drawnElements;
                }
            }
        };
    }

    std::optional<RendererPass> QuadRenderer::Draw(
        const std::vector<Raster::DrawText>& draws, const UniversalDataBuffer& universalDataBuffer)
    {
        if (draws.empty())
            return {};

        struct Group
        {
            const Asset::Resource::Font* fontResource = nullptr;
            Shaders shaders;
            std::vector<Quad> elements = {};
        };
        
        std::vector<Group> groups;
        auto currentGroup = &groups.emplace_back(
            draws.begin()->font->Resource(),
            draws.begin()->shaders,
            std::vector<Quad> {});
        for (auto& draw : draws)
        {
            if (draw.font->Resource() != currentGroup->fontResource || draw.shaders != currentGroup->shaders)
            {
                currentGroup = &groups.emplace_back(
                    draw.font->Resource(),
                    draw.shaders,
                    std::vector<Quad>{});
            }

            const auto quads = ToQuads(draw);
            currentGroup->elements.insert(currentGroup->elements.end(), quads.begin(), quads.end());
        }

        auto& descriptorSetPool = NextDescriptorSetPool();
        const auto descriptorSets = descriptorSetPool.Retrieve(groups.size());

        return RendererPass
        {
            Command
            {
                [this, groups, descriptorSets, &universalDataBuffer](CommandRecorder record)
                {
                    auto drawnElements = 0;

                    size_t i = 0;
                    for (auto& group : groups)
                    {
                        const auto descriptorSet = descriptorSets[i];

                        auto& descriptor = dynamic_cast<const Asset::Resource::Vulkan::Image*>(group.fontResource)->imageData.descriptor;
                        descriptor.Update(descriptorSet, device);
                        universalDataBuffer.Update(descriptorSet);

                        record(WriteData(group.elements, totalElements + drawnElements));

                        drawnElements += group.elements.size();
                        ++i;
                    }
                }
            },
            Command
            {
                [this, groups, descriptorSets](CommandRecorder record)
                {
                    auto drawnElements = 0;

                    size_t i = 0;
                    for (auto& group : groups)
                    {
                        const auto descriptorSet = descriptorSets[i];

                        record(Draw(
                            group.elements.size() * verticesPerQuad,
                            group.elements.size() * indicesPerQuad,
                            (totalElements + drawnElements) * indicesPerQuad,
                            mappedConduits.For(group.shaders),
                            descriptorSet));

                        drawnElements += group.elements.size();
                        ++i;
                    }

                    totalElements += drawnElements;
                }
            }
        };
    }
    
    Command QuadRenderer::WriteData(
        const std::vector<Quad>& elements, std::uint32_t startElementCount)
    {
        std::vector<QuadVertex> verticesToDraw;
        std::vector<QuadIndex> indicesToDraw;
        std::uint32_t currentQuad = 0;
        for (auto& element : elements)
        {
            verticesToDraw.insert(verticesToDraw.end(), element.vertices.begin(), element.vertices.end());
            const auto offsetIndicesBy = (startElementCount + currentQuad) * quadIndexIncrement;
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

        const auto vertexOffset = vk::DeviceSize(startElementCount) * verticesPerQuad * sizeof QuadVertex;
        const auto vertexSize = verticesToDraw.size() * sizeof QuadVertex;
        const auto indexOffset = vk::DeviceSize(startElementCount) * indicesPerQuad * sizeof QuadIndex;
        const auto indexSize = indicesToDraw.size() * sizeof QuadIndex;

        vertexBuffer.PushSourceBytes(verticesToDraw.data(), vertexOffset, vertexSize);
        indexBuffer.PushSourceBytes(indicesToDraw.data(), indexOffset, indexSize);

        return [this, vertexOffset, vertexSize, indexOffset, indexSize](CommandRecorder record)
        {
            record(vertexBuffer.CopyFromSourceToDestination(vertexOffset, vertexSize));
            record(indexBuffer.CopyFromSourceToDestination(indexOffset, indexSize));
        };
    }

    Command QuadRenderer::Draw(
        std::uint32_t vertexCount,
        std::uint32_t indexCount,
        std::uint32_t startIndexCount,
        Conduit& conduit,
        vk::DescriptorSet descriptorSet)
    {
        return [this, vertexCount, indexCount, startIndexCount, &conduit, descriptorSet](CommandRecorder record)
        {
            record(conduit.Bind());
            record([this, vertexCount, indexCount, startIndexCount, &conduit, descriptorSet](vk::CommandBuffer commandBuffer)
                {
                    const vk::Buffer vertexBuffers[] = { vertexBuffer.destination.Value() };
                    constexpr vk::DeviceSize offsets[] = { 0 };
                    commandBuffer.bindVertexBuffers(0, 1, vertexBuffers, offsets);
                    commandBuffer.bindIndexBuffer(indexBuffer.destination.Value(), 0, vk::IndexType::eUint16);

                    commandBuffer.bindDescriptorSets(
                        vk::PipelineBindPoint::eGraphics,
                        mappedConduits.PipelineLayout(),
                        0,
                        1,
                        &descriptorSet,
                        0,
                        nullptr);

                    commandBuffer.drawIndexed(indexCount, 1, startIndexCount, 0, 0);
                });
        };
    }

    std::vector<Quad> QuadRenderer::ToQuads(const Raster::DrawText& draw)
    {
        std::vector<Quad> quads;

        const auto linesResult = glyphAtlas->ToLines(
            { draw.font->Resource(), draw.bold, draw.italics}, draw.string, draw.wrapWidth);
        if (!linesResult)
            return {};

        const auto scalers = draw.scalers;
        const auto position = draw.position;
        const auto atlasImageSize = linesResult->imageSize;
        const auto atlasCellSize = linesResult->cellSize;
        const auto totalSize = Spatial::ScaleBy(linesResult->totalSize, draw.scalers);

        const auto topLeft = position - Spatial::Point2D{ totalSize.width / 2, totalSize.height / 2 };

        const auto slice = Clamp(draw.viewSlice, Spatial::ToAxisAlignedBox2D(0, 0, 1, 1));
        const auto maximumSlice = Spatial::ToAxisAlignedBox2D(
            slice.Left() * totalSize.width,
            slice.Top() * totalSize.height,
            slice.Right() * totalSize.width,
            slice.Bottom() * totalSize.height);

        auto x = 0.0f;
        auto y = 0.0f;
        for (auto& line : linesResult->glyphs)
        {
            for (auto& glyph : line)
            {
                const auto width = glyph.size.width;
                const auto height = glyph.size.height;
                const auto advance = glyph.advance;

                const auto glyphStandard = Spatial::ToAxisAlignedBox2D(x, y, x + width * scalers.x, y + height * scalers.y);
                const auto glyphSlice = Clamp(glyphStandard, maximumSlice);
                const auto glyphPercentage = ScaleOf(glyphSlice, glyphStandard);

                if (glyphSlice.size.width > 0.0f && glyphSlice.size.height > 0.0f)
                {
                    const auto texture = ScaleOf(
                        Spatial::ToAxisAlignedBox2D(
                            glyph.column * atlasCellSize.width + width * glyphPercentage.Left(),
                            glyph.row * atlasCellSize.height + height * glyphPercentage.Top(),
                            glyph.column * atlasCellSize.width + width * glyphPercentage.Right(),
                            glyph.row * atlasCellSize.height + height * glyphPercentage.Bottom()),
                        Spatial::ToAxisAlignedBox2D(0, 0, atlasImageSize.width, atlasImageSize.height));

                    const auto center = glyphSlice.center + topLeft;
                    quads.push_back(ToQuad(
                        draw.color,
                        center,
                        glyphSlice.size,
                        draw.rotation,
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
    
    std::vector<DescriptorSetPool> QuadRenderer::CreateDescriptorSetPools(vk::Device device)
    {
        std::vector<DescriptorSetPool> pools;
        for (size_t i = 0; i < maxFramesInFlight; ++i)
        {
            pools.emplace_back(
                std::vector
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

    DescriptorSetPool& QuadRenderer::NextDescriptorSetPool()
    {
        ++currentDescriptorSetPool;
        if (currentDescriptorSetPool == descriptorSetPools.end())
            currentDescriptorSetPool = descriptorSetPools.begin();
        return *currentDescriptorSetPool;
    }
}