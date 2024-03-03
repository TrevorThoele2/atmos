#include "AncillarySurface.h"

namespace Atmos::Render
{
    AncillarySurface::AncillarySurface(Init init, DataPtr&& data) : Surface(init, std::move(data))
    {}
}