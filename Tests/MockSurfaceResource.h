#pragma once

#include <Atmos/SurfaceResource.h>

class MockSurfaceResource final : public Atmos::Render::Resource::Surface
{
public:
    void OnMaterialDestroying(const Arca::Index<Atmos::Asset::Material>& material) override;

    [[nodiscard]] Atmos::Spatial::Size2D Size() const override;
};