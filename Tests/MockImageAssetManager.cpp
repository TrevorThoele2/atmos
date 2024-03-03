#include "MockImageAssetManager.h"

Atmos::Asset::Resource::Loaded<Atmos::Asset::Resource::Image> MockImageAssetManager::Load(
    const Atmos::File::Path& filePath)
{
    return
    {
        {},
        Atmos::Asset::ImageFileType::Png,
        {1, 1}
    };
}

Atmos::Asset::Resource::Loaded<Atmos::Asset::Resource::Image> MockImageAssetManager::Load(
    const Atmos::DataBuffer& memory)
{
    return
    {
        {},
        Atmos::Asset::ImageFileType::Png,
        {1, 1}
    };
}