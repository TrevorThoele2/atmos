#include "VulkanSurfaceResource.h"

namespace Atmos::Render::Resource::Vulkan
{
    Surface::Surface(BackingSurface& backing) : backing(&backing)
    {}
    
    [[nodiscard]] Spatial::Size2D Surface::Size() const
    {
        return backing->Size();
    }
}