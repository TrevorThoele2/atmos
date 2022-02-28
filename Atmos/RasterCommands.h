#pragma once

#include "RasterCommand.h"

namespace Atmos::Render::Raster
{
    using Commands = std::unordered_map<Resource::Surface*, std::vector<Command>>;
}