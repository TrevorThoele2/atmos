#include "VulkanGlyphAtlas.h"

#include "VulkanImage.h"
#include "VulkanCommandBuffer.h"
#include "VulkanSynchronization.h"
#include "VulkanBuffer.h"

#include <utf8.h>

namespace Atmos::Render::Vulkan
{
    GlyphAtlas::GlyphAtlas(
        vk::Device device,
        vk::CommandBuffer commandBuffer,
        vk::Queue queue,
        vk::Fence fence,
        vk::PhysicalDeviceMemoryProperties memoryProperties,
        Logging::Logger& logger)
        :
        device(device),
        commandBuffer(commandBuffer),
        queue(queue),
        fence(fence),
        memoryPool(0, memoryProperties, device),
        logger(&logger)
    {}

    auto GlyphAtlas::ToLines(const FontID& fontID, const String& string, float wrapWidth) -> std::optional<LinesResult>
    {
        if (string.empty())
            return {};

        const auto retrieveGlyph = [&](char32_t codepoint) -> Glyph
        {
            const auto result = GlyphFor(fontID, codepoint);
            return std::get<0>(result);
        };
        const auto lines = Render::ToLines(utf8::utf8to32(string), wrapWidth, retrieveGlyph);

        const auto& font = fonts.find(fontID)->second;
        const auto maximumDimensionPixels = font.maximumDimensionPixels;
        const auto level = font.level;

        const auto height = maximumDimensionPixels * lines.glyphs.size();
        const auto totalSize = Spatial::Size2D{ lines.width, static_cast<float>(height) };
        const auto imageSize = ImageSize(maximumDimensionPixels, level);
        const auto cellSize = Spatial::Size2D
        {
            static_cast<Spatial::Size2D::Value>(maximumDimensionPixels),
            static_cast<Spatial::Size2D::Value>(maximumDimensionPixels)
        };
        return LinesResult{ lines.glyphs, totalSize, imageSize, cellSize, font.image.get(), font.imageView.get() };
    }

    void GlyphAtlas::CreateLargerImage(Font& font)
    {
        const auto maximumDimensionPixels = font.maximumDimensionPixels;
        const auto newLevel = font.level + 1;
        const auto size = ImageSize(maximumDimensionPixels, newLevel);

        auto [newImage, newMemory] = CreateGlyphImage(size);
        auto oldImage = font.image.get();

        const auto fontDimensionPixels = FontDimensionPixels(maximumDimensionPixels, newLevel);
        Record(
            commandBuffer,
            vk::CommandBufferUsageFlagBits::eOneTimeSubmit,
            [&](auto record)
            {
                record(Copy(
                    oldImage,
                    vk::ImageLayout::eGeneral,
                    newImage.get(),
                    vk::ImageLayout::eGeneral,
                    {
                        vk::ImageCopy
                        {
                            {vk::ImageAspectFlagBits::eColor, 0, 0, 1},
                            {},
                            {vk::ImageAspectFlagBits::eColor, 0, 0, 1},
                            {},
                            vk::Extent3D
                            {
                                fontDimensionPixels,
                                fontDimensionPixels,
                                1
                            }
                        }
                    }));
            });
        Submit(commandBuffer, queue, fence);

        WaitAndReset(device, fence, *logger);

        ++font.level;
        font.image = std::move(newImage);
        font.memory = std::move(newMemory);
    }

    auto GlyphAtlas::AddGlyph(Font& font, const FontID& fontID, char32_t codepoint) -> GlyphResult
    {
        const auto currentGlyphs = font.glyphs.size();

        const auto glyphsPerQuadrant = GlyphsPerQuadrant(font.level);
        const auto quadrant = currentGlyphs / glyphsPerQuadrant;
        const auto columnInAtlas = quadrant % (quadrants / 2);
        const auto rowInAtlas = quadrant / (quadrants / 2);

        const auto indexInQuadrant = currentGlyphs % glyphsPerQuadrant;
        const auto columnInQuadrant = indexInQuadrant % startingGlyphsPerImageDimension;
        const auto rowInQuadrant = indexInQuadrant / startingGlyphsPerImageDimension;

        const auto column = columnInAtlas * glyphsPerQuadrant + columnInQuadrant;
        const auto row = rowInAtlas * glyphsPerQuadrant + rowInQuadrant;

        const auto destinationImage = font.image.get();

        const auto offset = vk::Offset3D
        {
            static_cast<std::int32_t>(column * font.maximumDimensionPixels),
            static_cast<std::int32_t>(row * font.maximumDimensionPixels),
            0
        };
        const auto [sourceSize, sourceAdvance] = MergeGlyph(fontID, codepoint, destinationImage, offset);

        const auto glyph = Glyph{ column, row, sourceSize, sourceAdvance };
        font.glyphs.emplace(codepoint, glyph);

        return { glyph, destinationImage };
    }

    auto GlyphAtlas::GlyphFor(const FontID& fontID, char32_t codepoint) -> GlyphResult
    {
        auto foundFont = fonts.find(fontID);
        if (foundFont == fonts.end())
        {
            const auto level = 0;
            const auto maximumDimensionPixels = static_cast<std::uint32_t>(fontID.resource->MaximumGlyphHeight());
            const auto size = ImageSize(maximumDimensionPixels, level);
            auto [image, memory] = CreateGlyphImage(size);
            auto imageView = CreateImageView(image.get(), device, defaultImageFormat, 0, 1);
            auto font = Font
            {
                level,
                static_cast<std::uint32_t>(maximumDimensionPixels),
                {},
                std::move(image),
                std::move(imageView),
                std::move(memory)
            };
            foundFont = fonts.emplace(fontID, std::move(font)).first;
        }

        auto& font = foundFont->second;

        auto foundGlyph = font.glyphs.find(codepoint);
        if (foundGlyph == font.glyphs.end())
        {
            const auto maximumGlyphs = GlyphsPerQuadrant(font.level) * quadrants;
            const auto currentGlyphs = font.glyphs.size();
            if (currentGlyphs == maximumGlyphs)
                CreateLargerImage(font);

            return AddGlyph(font, fontID, codepoint);
        }
        else
            return { foundGlyph->second, font.image.get() };
    }

    std::tuple<vk::UniqueImage, UniqueMemory> GlyphAtlas::CreateGlyphImage(const Spatial::Size2D size)
    {
        auto image = CreateImage(
            device,
            static_cast<std::uint32_t>(size.width),
            static_cast<std::uint32_t>(size.height),
            defaultImageFormat,
            vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eTransferSrc | vk::ImageUsageFlagBits::eSampled,
            InitialImageLayout::Undefined);

        auto memory = memoryPool.Bind(image.get(), vk::MemoryPropertyFlagBits::eDeviceLocal);

        Record(
            commandBuffer,
            vk::CommandBufferUsageFlagBits::eOneTimeSubmit,
            [&](CommandRecorder record)
            {
                record(TransitionLayout(*image, vk::ImageLayout::eUndefined, vk::ImageLayout::eGeneral, 0, 1));
                record(ClearImage(*image, vk::ImageLayout::eGeneral, Color{ 0, 0, 0, 0 }));
            });
        Submit(commandBuffer, queue, fence);

        WaitAndReset(device, fence, *logger);

        return { std::move(image), std::move(memory) };
    }

    auto GlyphAtlas::MergeGlyph(const FontID& fontID, char32_t codepoint, vk::Image to, vk::Offset3D offset) -> LoadedGlyph
    {
        auto [buffer, size, advance] = fontID.resource->Load(codepoint, fontID.bold, fontID.italics);

        const auto width = static_cast<uint32_t>(size.width != 0 ? size.width : advance);
        const auto height = static_cast<uint32_t>(size.height != 0 ? size.height : fontID.resource->MaximumGlyphHeight());

        Vulkan::Buffer stagingBuffer(
            vk::DeviceSize(buffer.size()),
            vk::BufferUsageFlagBits::eTransferSrc,
            vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
            device,
            memoryPool);
        stagingBuffer.PushBytes(buffer, 0);

        Record(
            commandBuffer,
            vk::CommandBufferUsageFlagBits::eOneTimeSubmit,
            [&](auto record)
            {
                record(stagingBuffer.Copy(
                    to,
                    0,
                    0,
                    0,
                    offset,
                    { width, height, 1 },
                    0,
                    1));
            });
        Submit(commandBuffer, queue, fence);

        WaitAndReset(device, fence, *logger);

        return { size, advance };
    }

    std::uint32_t GlyphAtlas::GlyphsPerQuadrant(std::uint32_t level)
    {
        return startingGlyphsPerImage + startingGlyphsPerImage * level;
    }

    std::uint32_t GlyphAtlas::FontDimensionPixels(std::uint32_t maximumDimensionPixels, std::uint32_t level)
    {
        const auto base = startingGlyphsPerImageDimension * maximumDimensionPixels;
        return base + base * level;
    }

    Spatial::Size2D GlyphAtlas::ImageSize(std::uint32_t maximumDimensionPixels, std::uint32_t level) const
    {
        const auto baseDimension = maximumDimensionPixels * startingGlyphsPerImageDimension;
        const auto dimension = baseDimension + baseDimension * level;
        return { static_cast<float>(dimension), static_cast<float>(dimension) };
    }
}