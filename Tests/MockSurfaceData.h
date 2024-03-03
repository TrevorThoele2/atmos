#pragma once

#include <Atmos/SurfaceData.h>

class MockSurfaceDataImplementation final : public Atmos::Render::SurfaceData
{
public:
    std::vector<Atmos::Render::ImageRender> imageRenders;
    std::vector<Atmos::Render::LineRender> lineRenders;
public:
    void StageRender(const Atmos::Render::ImageRender& imageRender) override;
    void StageRender(const Atmos::Render::LineRender& lineRender) override;

    void DrawFrame(const Atmos::Render::Color& backgroundColor) override;

    [[nodiscard]] Atmos::ScreenSize Size() const override;
};