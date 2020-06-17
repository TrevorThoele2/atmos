#pragma once

#include <Atmos/SurfaceResource.h>

class MockSurfaceResourceImplementation final : public Atmos::Render::Resource::Surface
{
public:
    std::vector<Atmos::Render::ImageRender> imageRenders;
    std::vector<Atmos::Render::LineRender> lineRenders;
public:
    void StageRender(const Atmos::Render::ImageRender& imageRender) override;
    void StageRender(const Atmos::Render::LineRender& lineRender) override;

    void DrawFrame(Arca::Reliquary& reliquary, const Atmos::Render::Color& backgroundColor) override;

    [[nodiscard]] Atmos::ScreenSize Size() const override;
};