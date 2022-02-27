#pragma once

#include <tuple>
#include "RasterOrder.h"

namespace Atmos::Render::Raster
{
    template<class T>
    using Ordered = std::tuple<T, Order>;
}