#include "SDLText.h"

#include "SDLTextError.h"
#include <utf8.h>
#include "MathUtility.h"

namespace Atmos::Render::SDL
{
    void SetStyle(TTF_Font& font, int style)
    {
        if (TTF_GetFontStyle(&font) != style)
            TTF_SetFontStyle(&font, style);
    }

    int Style(bool bold, bool italics)
    {
        auto style = 0;
        if (bold)
            style |= TTF_STYLE_BOLD;
        if (italics)
            style |= TTF_STYLE_ITALIC;
        return style;
    }

    Spatial::Size2D Size(TTF_Font& font, const std::u32string& string)
    {
        int width = 0;
        int height = 0;
        const auto text = utf8::utf32to8(string);
        VerifyResult(
            TTF_SizeUTF8(&font, text.c_str(), &width, &height),
            "Could not find size of text.");

        return Spatial::Size2D
        {
            static_cast<Spatial::Size2D::Value>(width),
            static_cast<Spatial::Size2D::Value>(height)
        };
    }

    Spatial::Size2D Size(TTF_Font& font, const std::vector<std::u32string>& strings)
    {
        auto width = 0.0f;
        auto height = 0.0f;

        for (auto& string : strings)
        {
            const auto currentSize = Size(font, string);
            width = std::max(currentSize.width, width);
            height += currentSize.height;
        }

        return Spatial::Size2D{ width, height };
    }

    SDL_Color Color(const Render::Color& color)
    {
        return SDL_Color{ color.red, color.green, color.blue, color.alpha };
    }

    Uint32 ConvertWrapWidth(float wrapWidth)
    {
        return static_cast<Uint32>(ConvertRange(
            static_cast<double>(wrapWidth),
            static_cast<double>(std::numeric_limits<float>::lowest()),
            static_cast<double>(std::numeric_limits<float>::max()),
            static_cast<double>(std::numeric_limits<Uint32>::lowest()),
            static_cast<double>(std::numeric_limits<Uint32>::max())));
    }
}