#pragma once

#include <Atmos/SurfaceResource.h>

class MockSurfaceResource final : public Atmos::Render::Resource::Surface
{
public:
    [[nodiscard]] Atmos::Spatial::Size2D Size() const override;
};