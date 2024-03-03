#include "SDLFontAssetResource.h"

#include "GraphicsError.h"
#include "SDLText.h"
#include <utf8.h>
#include "SDLTextError.h"

namespace Atmos::Render::SDL
{
    FontAssetResource::FontAssetResource(const String& name, const Buffer& buffer) :
        name(name), buffer(buffer)
    {
        ops = SDL_RWFromMem(this->buffer.data(), this->buffer.size());
        if (!ops)
            throw GraphicsError("Could not create font asset resource.", { {"Reason", String(SDL_GetError())} });
        
        font = TTF_OpenFontRW(ops, 0, 18);
        if (!font)
            throw GraphicsError(
                "Could not open TTF font.",
                {
                    { "Name", name },
                    { "Reason", String(TTF_GetError()) }
                });

        maximumHeight = TTF_FontHeight(font);
    }

    FontAssetResource::~FontAssetResource()
    {
        if (font)
            TTF_CloseFont(font);
    }

    LoadedGlyph FontAssetResource::Load(char32_t codepoint, bool bold, bool italics) const
    {
        SetStyle(*font, Style(bold, italics));

        const auto metrics = Metrics(codepoint);
        auto text = std::array<char, 5>{};
        memcpy(&text, &codepoint, 4);
        const auto surface = TTF_RenderUTF8_Blended(font, text.data(), Color(Render::Color(255, 255, 255, 255)));
        if (surface->w == 0 || surface->h == 0)
            return { {}, Spatial::Size2D{0, 0} };

        SDL_LockSurface(surface);

        const auto byteCount = surface->w * surface->h * surface->format->BytesPerPixel;

        Buffer buffer;
        buffer.resize(byteCount);

        const auto pixels = static_cast<char*>(surface->pixels);
        memcpy(&buffer[0], pixels, byteCount);

        const auto size = Spatial::Size2D
        {
            static_cast<Spatial::Size2D::Value>(surface->w),
            static_cast<Spatial::Size2D::Value>(surface->h)
        };

        SDL_UnlockSurface(surface);
        SDL_FreeSurface(surface);

        return { buffer, size, static_cast<float>(metrics.advance) };
    }

    float FontAssetResource::MaximumGlyphHeight() const
    {
        return static_cast<float>(maximumHeight);
    }

    auto FontAssetResource::Metrics(char32_t codepoint) const -> GlyphMetrics
    {
        std::int32_t minX = 0;
        std::int32_t maxX = 0;
        std::int32_t minY = 0;
        std::int32_t maxY = 0;
        std::int32_t advance = 0;

        VerifyResult(
            TTF_GlyphMetrics(font, static_cast<Uint16>(codepoint), &minX, &maxX, &minY, &maxY, &advance),
            "Could not query for glyph metrics.");

        return
        {
            minX,
            maxX,
            minY,
            maxY,
            maxX - minX,
            maxY - minY,
            advance
        };
    }
}