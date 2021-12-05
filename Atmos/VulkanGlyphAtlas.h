#pragma once

#include "VulkanIncludes.h"
#include "FontID.h"
#include "Glyph.h"
#include "GlyphAlgorithms.h"
#include "VulkanMemoryPool.h"
#include "Size2D.h"

#include "Logger.h"

namespace Atmos::Render::Vulkan
{
	class GlyphAtlas
	{
    public:
        struct LinesResult
        {
            std::vector<std::vector<Glyph>> glyphs;
            Spatial::Size2D totalSize;
            Spatial::Size2D imageSize;
            Spatial::Size2D cellSize;
            vk::Image image;
            vk::ImageView imageView;
        };
	public:
        GlyphAtlas(
            vk::Device device,
            vk::CommandBuffer commandBuffer,
            vk::Queue queue,
            vk::Fence fence,
            vk::PhysicalDeviceMemoryProperties memoryProperties,
            Logging::Logger& logger);

        [[nodiscard]] std::optional<LinesResult> ToLines(const FontID& fontID, const String& string, float wrapWidth);
	private:
        struct Font
        {
            std::uint32_t level = 0;
            std::uint32_t maximumDimensionPixels = 0;
            std::unordered_map<char32_t, Glyph> glyphs;
            vk::UniqueImage image;
            vk::UniqueImageView imageView;
            UniqueMemory memory;
        };

        static const std::uint32_t startingGlyphsPerImageDimension = 16;
        static const std::uint32_t startingGlyphsPerImage = startingGlyphsPerImageDimension * startingGlyphsPerImageDimension;
        static const std::uint32_t quadrants = 4;

        std::unordered_map<FontID, Font> fonts;
    private:
        using GlyphResult = std::tuple<Glyph, vk::Image>;

        struct LoadedGlyph
        {
            Spatial::Size2D size;
            float advance;
        };

        void CreateLargerImage(Font& font);
        GlyphResult AddGlyph(Font& font, const FontID& fontID, char32_t codepoint);

        [[nodiscard]] GlyphResult GlyphFor(const FontID& fontID, char32_t codepoint);
        [[nodiscard]] std::tuple<vk::UniqueImage, UniqueMemory> CreateGlyphImage(const Spatial::Size2D size);
        [[nodiscard]] LoadedGlyph MergeGlyph(const FontID& fontID, char32_t codepoint, vk::Image to, vk::Offset3D offset);
        [[nodiscard]] std::uint32_t GlyphsPerQuadrant(std::uint32_t level);
        [[nodiscard]] std::uint32_t FontDimensionPixels(std::uint32_t maximumDimensionPixels, std::uint32_t level);
        [[nodiscard]] Spatial::Size2D ImageSize(std::uint32_t maximumDimensionPixels, std::uint32_t level) const;
    private:
        vk::Device device;
        vk::CommandBuffer commandBuffer;
        vk::Queue queue;
        vk::Fence fence;
        MemoryPool memoryPool;
        Logging::Logger* logger;
	};
}