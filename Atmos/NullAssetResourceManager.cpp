#include "NullAssetResourceManager.h"

namespace Atmos::Asset::Resource
{
    LoadedData<Image> NullManager::LoadImageData(const File::Path& filePath)
    {
        return {};
    }

    LoadedData<Image> NullManager::LoadImageData(const Buffer& memory)
    {
        return {};
    }

    LoadedData<Audio> NullManager::LoadAudioData(const File::Path& filePath)
    {
        return {};
    }

    LoadedData<Audio> NullManager::LoadAudioData(const Buffer& memory)
    {
        return {};
    }
}