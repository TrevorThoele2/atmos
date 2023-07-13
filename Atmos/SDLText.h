#pragma once

#include "SDLIncludes.h"
#include "Size2D.h"
#include "String.h"
#include "Color.h"

namespace Atmos::Render::SDL
{
    void SetStyle(TTF_Font& font, int style);
    [[nodiscard]] Spatial::Size2D Size(TTF_Font& font, const std::u32string& string);
    [[nodiscard]] Spatial::Size2D Size(TTF_Font& font, const std::vector<std::u32string>& strings);
    [[nodiscard]] int Style(bool bold, bool italics);
    [[nodiscard]] SDL_Color Color(const Render::Color& color);
    [[nodiscard]] Uint32 ConvertWrapWidth(float wrapWidth);
}
