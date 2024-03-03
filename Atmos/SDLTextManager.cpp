#include "SDLTextManager.h"

#include "GraphicsError.h"

namespace Atmos::Render::SDL
{
    TextManager::TextManager(Logging::Logger& logger)
    {
		const auto initResult = TTF_Init();
		if (initResult == -1)
            throw GraphicsError(
                "Could not initialize SDL text manager.",
                {
                    {"Reason", TTF_GetError()}
                });
    }

	std::unique_ptr<Asset::Resource::Font> TextManager::CreateFontResource(const Buffer& buffer, const Name& name) const
    {
		return std::make_unique<FontAssetResource>(name, buffer);
    }

	TextData TextManager::DataFor(Asset::Resource::Font& font, const String& string, float wrapWidth, bool bold, bool italics)
	{
        const auto sdlAsset = dynamic_cast<FontAssetResource*>(&font);
		if (!sdlAsset)
			throw GraphicsError("Font asset resource was not the correct type.");

        const auto sdlFont = sdlAsset->font;

        const auto style = StyleFrom(bold, italics);
        if (TTF_GetFontStyle(sdlFont) != style)
            TTF_SetFontStyle(sdlFont, style);

        const auto useColor = ColorFrom(Color{255, 255, 255, 255});
        const auto useWrapWidth = static_cast<Uint32>(wrapWidth);
        const auto surface = Surface(*sdlFont, string, useColor, useWrapWidth);
        auto textData = ExtractData(*surface);
        SDL_FreeSurface(surface);

        return textData;
	}

    SDL_Surface* TextManager::Surface(TTF_Font& font, const String& string, const SDL_Color& color, Uint32 wrapWidth)
    {
        const auto size = Size(font, string);
        const auto surface = static_cast<Uint32>(size.width) <= wrapWidth
            ? TTF_RenderText_Blended(&font, string.c_str(), color)
            : TTF_RenderText_Blended_Wrapped(&font, string.c_str(), color, wrapWidth);
        if (!surface)
            throw GraphicsError("Could not create surface from text.", { {"Reason", TTF_GetError()} });
        return surface;
    }

    TextData TextManager::ExtractData(SDL_Surface& surface)
    {
        SDL_LockSurface(&surface);

        const auto pixelCount = surface.w * surface.h;

        Buffer buffer;
        buffer.resize(pixelCount * surface.format->BytesPerPixel);

        const auto pixels = static_cast<char*>(surface.pixels);
        for (auto currentPixel = 0; currentPixel < pixelCount; ++currentPixel)
        {
            for (auto byte = 0; byte < surface.format->BytesPerPixel; ++byte)
            {
                const auto currentPixelBytes = surface.format->BytesPerPixel * currentPixel + byte;
                buffer[currentPixelBytes] = pixels[currentPixelBytes];
            }
        }

        SDL_UnlockSurface(&surface);

        const auto size = Spatial::Size2D
        {
            static_cast<Spatial::Size2D::Value>(surface.w),
            static_cast<Spatial::Size2D::Value>(surface.h)
        };

        return { buffer, size };
    }

    Spatial::Size2D TextManager::Size(TTF_Font& font, const String& string)
    {
        int width = 0;
        int height = 0;
        const auto result = TTF_SizeText(&font, string.c_str(), &width, &height);
        if (result < 0)
            throw GraphicsError("Could not find size of text.", { {"Reason", TTF_GetError()} });

        return Spatial::Size2D{ static_cast<Spatial::Size2D::Value>(width), static_cast<Spatial::Size2D::Value>(height) };
    }

    int TextManager::StyleFrom(bool bold, bool italics)
    {
        auto style = 0;
        if (bold)
            style |= TTF_STYLE_BOLD;
        if (italics)
            style |= TTF_STYLE_ITALIC;
        return style;
    }

    SDL_Color TextManager::ColorFrom(const Color& color)
    {
        return SDL_Color{ color.red, color.green, color.blue, color.alpha };
    }
}