#include "VulkanSurfaceResource.h"

namespace Atmos::Render::Resource::Vulkan
{
    Surface::Surface(BackingSurface& backing) : backing(&backing)
    {}

    void Surface::StageRender(const ImageRender& imageRender)
    {
        backing->StageRender(imageRender);
    }

    void Surface::StageRender(const LineRender& lineRender)
    {
        backing->StageRender(lineRender);
    }

    void Surface::StageRender(const RegionRender& regionRender)
    {
        backing->StageRender(regionRender);
    }

    void Surface::StageRender(const TextRender& textRender)
    {
        backing->StageRender(textRender);
    }

    void Surface::DrawFrame(const Spatial::Point2D& mapPosition, const Color& backgroundColor)
    {
        backing->DrawFrame(mapPosition, backgroundColor);
    }

    void Surface::OnMaterialDestroying(const Arca::Index<Asset::Material>& material)
    {
        backing->OnMaterialDestroying(material);
    }

    [[nodiscard]] Spatial::Size2D Surface::Size() const
    {
        return backing->Size();
    }
}