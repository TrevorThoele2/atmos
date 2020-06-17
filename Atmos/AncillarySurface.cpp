#include "AncillarySurface.h"

namespace Atmos::Render
{
    AncillarySurface::AncillarySurface(Init init, ResourcePtr&& resource) : Surface(init, std::move(resource))
    {}
}