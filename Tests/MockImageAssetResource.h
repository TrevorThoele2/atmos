#pragma once

#include <Atmos/ImageAssetResource.h>

using namespace Atmos;

class MockImageAssetResource final : public Asset::Resource::Image
{
public:
    Spatial::Size2D size;

    MockImageAssetResource();
    MockImageAssetResource(Spatial::Size2D size);

    [[nodiscard]] Spatial::Size2D Size() const override;
};