#pragma once

#include "RenderImage.h"
#include "RenderLine.h"
#include "RenderText.h"
#include "RenderRegion.h"

namespace Atmos::Render
{
    struct AllRenders
    {
        std::vector<RenderImage> images;
        std::vector<RenderLine> lines;
        std::vector<RenderText> texts;
        std::vector<RenderRegion> regions;
    };
}