#include "SurfaceCore.h"

namespace Atmos::Render
{
    SurfaceCore::SurfaceCore(ResourcePtr&& resource) : resource(std::move(resource))
    {}
}