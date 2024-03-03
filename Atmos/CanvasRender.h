#pragma once

#include "Canvas.h"
#include "Position3D.h"
#include "Size2D.h"

namespace Atmos::Render
{
    struct CanvasRender
    {
        Canvas& canvas;

        Position3D position;
        Size2D size;
    };
}