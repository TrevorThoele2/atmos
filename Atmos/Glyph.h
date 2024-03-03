#pragma once

#include <numeric>
#include "Size2D.h"

namespace Atmos::Render
{
    struct Glyph
    {
        std::uint32_t column;
        std::uint32_t row;
        
        Spatial::Size2D size;
        float advance;
    };
}