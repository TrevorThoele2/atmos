#pragma once

#include <Atmos/AssetResourceManager.h>

class MockAssetResourceManager final : public Atmos::Asset::Resource::Manager
{
public:
    MockAssetResourceManager() = default;

    Atmos::Asset::Resource::LoadedData<Atmos::Asset::Resource::Image> LoadImageData(const Atmos::File::Path& filePath) override;
    Atmos::Asset::Resource::LoadedData<Atmos::Asset::Resource::Image> LoadImageData(const Atmos::Buffer& memory) override;

    Atmos::Asset::Resource::LoadedData<Atmos::Asset::Resource::Audio> LoadAudioData(const Atmos::File::Path& filePath) override;
    Atmos::Asset::Resource::LoadedData<Atmos::Asset::Resource::Audio> LoadAudioData(const Atmos::Buffer& memory) override;
};