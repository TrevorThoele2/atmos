#pragma once

#include "AssetResourceManager.h"

namespace Atmos::Asset::Resource
{
    class NullManager final : public Manager
    {
    public:
        NullManager(Logging::Logger& logger);

        LoadedData<Image> LoadImageData(const File::Path& filePath) override;
        LoadedData<Image> LoadImageData(const Buffer& memory) override;

        LoadedData<Audio> LoadAudioData(const File::Path& filePath) override;
        LoadedData<Audio> LoadAudioData(const Buffer& memory) override;
    };
}