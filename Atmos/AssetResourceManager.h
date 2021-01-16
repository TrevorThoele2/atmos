#pragma once

#include "LoadImageAssetResourceData.h"
#include "LoadAudioAssetResourceData.h"

namespace Atmos::Asset::Resource
{
    class Manager
    {
    public:
        virtual ~Manager() = 0;

        virtual LoadedData<Image> LoadImageData(const File::Path& filePath) = 0;
        virtual LoadedData<Image> LoadImageData(const Buffer& memory) = 0;

        virtual LoadedData<Audio> LoadAudioData(const File::Path& filePath) = 0;
        virtual LoadedData<Audio> LoadAudioData(const Buffer& memory) = 0;
    };
}