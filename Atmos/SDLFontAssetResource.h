#pragma once

#include "FontAssetResource.h"
#include "SDLIncludes.h"
#include "Buffer.h"

namespace Atmos::Render::SDL
{
    class FontAssetResource final : public Asset::Resource::Font
    {
    public:
        String name;

        TTF_Font* font;

        Buffer buffer;
        SDL_RWops* ops;
    public:
        FontAssetResource(const String& name, const Buffer& buffer);
        ~FontAssetResource();

        [[nodiscard]] LoadedGlyph Load(char32_t codepoint, bool bold, bool italics) const override;
        [[nodiscard]] float MaximumGlyphHeight() const override;
    private:
        int maximumHeight;
    private:
        struct GlyphMetrics
        {
            std::int32_t minX;
            std::int32_t maxX;
            std::int32_t minY;
            std::int32_t maxY;
            std::int32_t width;
            std::int32_t height;
            std::int32_t advance;
        };

        [[nodiscard]] GlyphMetrics Metrics(char32_t codepoint) const;
    };
}