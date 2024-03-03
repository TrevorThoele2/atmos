#pragma once

#include <Atmos/SurfaceResource.h>

class MockSurfaceResource final : public Atmos::Render::Resource::Surface
{
public:
    std::vector<Atmos::Render::ImageRender> imageRenders;
    std::vector<Atmos::Render::LineRender> lineRenders;
    std::vector<Atmos::Render::RegionRender> regionRenders;
public:
    void StageRender(const Atmos::Render::ImageRender& imageRender) override;
    void StageRender(const Atmos::Render::LineRender& lineRender) override;
    void StageRender(const Atmos::Render::RegionRender& regionRender) override;

    void DrawFrame(Arca::Reliquary& reliquary, const Atmos::Render::Color& backgroundColor) override;

    void OnMaterialCreated(const Arca::Index<Atmos::Asset::Material>& material) override;
    void OnMaterialDestroying(const Arca::Index<Atmos::Asset::Material>& material) override;

    [[nodiscard]] Atmos::Spatial::ScreenSize Size() const override;
};