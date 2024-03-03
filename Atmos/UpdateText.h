#pragma once

#include "TextResource.h"
#include "Buffer.h"
#include "Size2D.h"

namespace Atmos::Render
{
    struct UpdateText
    {
        Resource::Text* text;
        Buffer buffer;
        Spatial::Size2D size;
    };
}
