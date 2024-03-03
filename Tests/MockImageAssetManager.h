#pragma once

#include <Atmos/ImageAssetManager.h>

class MockImageAssetManager final : public Atmos::Asset::ImageManager
{
public:
    MockImageAssetManager() = default;

    Atmos::Asset::Resource::Loaded<Atmos::Asset::Resource::Image> Load(const Atmos::File::Path& filePath) override;
    Atmos::Asset::Resource::Loaded<Atmos::Asset::Resource::Image> Load(const Atmos::Buffer& memory) override;
};