#pragma once

#include <tuple>
#include "RasterOrder.h"
#include "SurfaceResource.h"

namespace Atmos::Render::Raster
{
    template<class T>
    using Prepared = std::tuple<T, Resource::Surface*, Order>;
}