#pragma once

#include "TextManager.h"
#include <SDL_ttf.h>
#include "SDLFontAssetResource.h"
#include "Color.h"
#include "Logger.h"

namespace Atmos::Render::SDL
{
    class TextManager final : public Render::TextManager
    {
    public:
        TextManager(Logging::Logger& logger);

        [[nodiscard]] std::unique_ptr<Asset::Resource::Font> CreateFontResource(
            const Buffer& buffer, const Name& name) const override;

        [[nodiscard]] TextData DataFor(
            Asset::Resource::Font& font, const String& string, float wrapWidth, bool bold, bool italics) override;
    private:
        [[nodiscard]] static SDL_Surface* Surface(
            TTF_Font& font, const std::vector<String>& strings, const SDL_Color& color, Uint32 wrapWidth);
        [[nodiscard]] static TextData ExtractData(SDL_Surface& surface);
        [[nodiscard]] static int StyleFrom(bool bold, bool italics);
        [[nodiscard]] static SDL_Color ColorFrom(const Color& color);
    };
}