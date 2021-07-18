#pragma once

#include "VulkanBackingSurfaceResource.h"

namespace Atmos::Render::Resource::Vulkan
{
    class Surface final : public Resource::Surface
    {
    public:
        BackingSurface* backing;

        Surface(BackingSurface& backing);

        void StageRender(const ImageRender& imageRender) override;
        void StageRender(const LineRender& lineRender) override;
        void StageRender(const RegionRender& regionRender) override;
        void StageRender(const TextRender& textRender) override;

        void DrawFrame(const Spatial::Point2D& mapPosition, const Color& backgroundColor) override;

        void OnMaterialDestroying(const Arca::Index<Asset::Material>& material) override;

        [[nodiscard]] Spatial::Size2D Size() const override;
    };
}