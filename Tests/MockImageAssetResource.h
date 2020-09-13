#pragma once

#include <Atmos/ImageAssetResource.h>

using namespace Atmos;

class MockImageAssetResource final : public Asset::Resource::Image
{
public:
    MockImageAssetResource() = default;

    [[nodiscard]] Asset::ImageSize Size() const override;
};