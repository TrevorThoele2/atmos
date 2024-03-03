#include "VulkanSurfaceResource.h"

namespace Atmos::Render::Resource::Vulkan
{
    Surface::Surface(BackingSurface& backing) : backing(&backing)
    {}
    
    void Surface::OnMaterialDestroying(const Arca::Index<Asset::Material>& material)
    {
        backing->OnMaterialDestroying(material);
    }

    [[nodiscard]] Spatial::Size2D Surface::Size() const
    {
        return backing->Size();
    }
}