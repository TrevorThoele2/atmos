#pragma once

#include "DrawImage.h"
#include "DrawLine.h"
#include "DrawRegion.h"
#include "DrawText.h"

namespace Atmos::Render::Raster
{
    using Command = std::variant<
        DrawImage,
        DrawLine,
        DrawRegion,
        DrawText>;
}