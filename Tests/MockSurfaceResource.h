#pragma once

#include <Atmos/SurfaceResource.h>

class MockSurfaceResource final : public Atmos::Render::Resource::Surface
{
public:
    std::vector<Atmos::Render::ImageRender> imageRenders;
    std::vector<Atmos::Render::LineRender> lineRenders;
    std::vector<Atmos::Render::RegionRender> regionRenders;
    std::vector<Atmos::Render::TextRender> textRenders;
public:
    void StageRender(const Atmos::Render::ImageRender& imageRender) override;
    void StageRender(const Atmos::Render::LineRender& lineRender) override;
    void StageRender(const Atmos::Render::RegionRender& regionRender) override;
    void StageRender(const Atmos::Render::TextRender& textRender) override;

    void DrawFrame(Arca::Reliquary& reliquary, const Atmos::Render::Color& backgroundColor) override;
    
    void OnMaterialDestroying(const Arca::Index<Atmos::Asset::Material>& material) override;

    [[nodiscard]] Atmos::Spatial::Size2D Size() const override;
};