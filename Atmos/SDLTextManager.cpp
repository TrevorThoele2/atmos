#include "SDLTextManager.h"

#include "GraphicsError.h"
#include "SDLText.h"
#include "MathUtility.h"

namespace Atmos::Render::SDL
{
    TextManager::TextManager()
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
        if (!string.empty())
        {
            const auto sdlAsset = dynamic_cast<FontAssetResource*>(&font);
            if (!sdlAsset)
                throw GraphicsError("Font asset resource was not the correct type.");

            const auto sdlFont = sdlAsset->font;
            
            SetStyle(*sdlFont, Style(bold, italics));

            const auto useColor = ColorFrom(Color{ 255, 255, 255, 255 });
            const auto useWrapWidth = static_cast<Uint32>(ConvertRange(
                static_cast<double>(wrapWidth),
                static_cast<double>(std::numeric_limits<float>::lowest()),
                static_cast<double>(std::numeric_limits<float>::max()),
                static_cast<double>(std::numeric_limits<Uint32>::lowest()),
                static_cast<double>(std::numeric_limits<Uint32>::max())));
            const auto strings = Split(string);
            const auto surface = Surface(*sdlFont, strings, useColor, useWrapWidth);
            auto textData = ExtractData(*surface);
            SDL_FreeSurface(surface);

            return textData;
        }
        else
            return { {}, Spatial::Size2D{0, 0} };
	}

    SDL_Surface* TextManager::Surface(
        TTF_Font& font, const std::vector<String>& strings, const SDL_Color& color, Uint32 wrapWidth)
    {
        auto width = 0;
        auto height = 0;

        std::vector<SDL_Surface*> surfaces;
        for (auto& string : strings)
        {
            const auto size = Size(font, string);
            const auto surface = static_cast<Uint32>(size.width) <= wrapWidth
                ? TTF_RenderText_Blended(&font, string.c_str(), color)
                : TTF_RenderText_Blended_Wrapped(&font, string.c_str(), color, wrapWidth);
            if (!surface)
                throw GraphicsError("Could not create surface from text.", { {"Reason", TTF_GetError()} });

            width = std::max(surface->w, width);
            height += surface->h;
            surfaces.push_back(surface);
        }

        const auto firstSurface = surfaces[0];

        auto currentHeight = 0;
        const auto surface = SDL_CreateRGBSurface(
            0,
            width,
            height,
            firstSurface->format->BitsPerPixel,
            firstSurface->format->Rmask,
            firstSurface->format->Gmask,
            firstSurface->format->Bmask,
            firstSurface->format->Amask);
        for(const auto& copyFrom : surfaces)
        {
            SDL_Rect destinationRectangle{ 0, currentHeight, 0, 0 };
            SDL_BlitSurface(copyFrom, nullptr, surface, &destinationRectangle);
            currentHeight += copyFrom->h;
            SDL_FreeSurface(copyFrom);
        }

        return surface;
    }

    TextData TextManager::ExtractData(SDL_Surface& surface)
    {
        SDL_LockSurface(&surface);

        const auto pixelCount = surface.w * surface.h;
        const auto byteCount = pixelCount * surface.format->BytesPerPixel;

        Buffer buffer;
        buffer.resize(byteCount);

        const auto pixels = static_cast<char*>(surface.pixels);
        memcpy(&buffer[0], pixels, byteCount);

        SDL_UnlockSurface(&surface);

        const auto size = Spatial::Size2D
        {
            static_cast<Spatial::Size2D::Value>(surface.w),
            static_cast<Spatial::Size2D::Value>(surface.h)
        };

        return { buffer, size };
    }

    SDL_Color TextManager::ColorFrom(const Color& color)
    {
        return SDL_Color{ color.red, color.green, color.blue, color.alpha };
    }
}