#include "MockAssetResourceManager.h"

Atmos::Asset::Resource::LoadedData<Atmos::Asset::Resource::Image> MockAssetResourceManager::LoadImageData(
    const Atmos::File::Path& filePath)
{
    return
    {
        {},
        Atmos::Asset::ImageFileType::Png,
        {1, 1}
    };
}

Atmos::Asset::Resource::LoadedData<Atmos::Asset::Resource::Image> MockAssetResourceManager::LoadImageData(
    const Atmos::Buffer& memory)
{
    return
    {
        {},
        Atmos::Asset::ImageFileType::Png,
        {1, 1}
    };
}

Atmos::Asset::Resource::LoadedData<Atmos::Asset::Resource::Audio> MockAssetResourceManager::LoadAudioData(
    const Atmos::File::Path& filePath)
{
    return
    {
        {}
    };
}

Atmos::Asset::Resource::LoadedData<Atmos::Asset::Resource::Audio> MockAssetResourceManager::LoadAudioData(
    const Atmos::Buffer& memory)
{
    return
    {
        {}
    };
}