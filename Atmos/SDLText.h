#pragma once

#include <SDL_ttf.h>
#include "Size2D.h"
#include "String.h"

namespace Atmos::Render::SDL
{
    [[nodiscard]] Spatial::Size2D Size(TTF_Font& font, const String& string);
    [[nodiscard]] Spatial::Size2D Size(TTF_Font& font, const std::vector<String>& strings);
    [[nodiscard]] std::vector<String> Split(const String& string);
    [[nodiscard]] int Style(bool bold, bool italics);
    void SetStyle(TTF_Font& font, int style);
}
