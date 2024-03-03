#pragma once

#include <Atmos/ImageAssetResource.h>

using namespace Atmos;

class MockImageAssetResource final : public Asset::Resource::Image
{
public:
    Asset::ImageSize size;

    MockImageAssetResource();
    MockImageAssetResource(Asset::ImageSize size);

    [[nodiscard]] Asset::ImageSize Size() const override;
};