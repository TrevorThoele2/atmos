#include "SDLText.h"

#include "GraphicsError.h"

namespace Atmos::Render::SDL
{
    int Style(bool bold, bool italics)
    {
        auto style = 0;
        if (bold)
            style |= TTF_STYLE_BOLD;
        if (italics)
            style |= TTF_STYLE_ITALIC;
        return style;
    }

    Spatial::Size2D Size(TTF_Font& font, const String& string)
    {
        int width = 0;
        int height = 0;
        const auto result = TTF_SizeText(&font, string.c_str(), &width, &height);
        if (result < 0)
            throw GraphicsError("Could not find size of text.", { {"Reason", TTF_GetError()} });

        return Spatial::Size2D
        {
            static_cast<Spatial::Size2D::Value>(width),
            static_cast<Spatial::Size2D::Value>(height)
        };
    }

    Spatial::Size2D Size(TTF_Font& font, const std::vector<String>& strings)
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

    std::vector<String> Split(const String& string)
    {
        return Chroma::Split(string, "\n");
    }

    void SetStyle(TTF_Font& font, int style)
    {
        if (TTF_GetFontStyle(&font) != style)
            TTF_SetFontStyle(&font, style);
    }
}