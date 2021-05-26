#pragma once

#include "Buffer.h"
#include "Size2D.h"

namespace Atmos::Render
{
    struct TextData
    {
        Buffer buffer;
        Spatial::Size2D size;
    };
}