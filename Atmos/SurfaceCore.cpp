#include "SurfaceCore.h"

namespace Atmos::Render
{
    SurfaceCore::SurfaceCore(ResourcePtr&& resource) : resource(std::move(resource))
    {}

    auto SurfaceCore::Resource() -> ResourceT*
    {
        return resource.get();
    }
}