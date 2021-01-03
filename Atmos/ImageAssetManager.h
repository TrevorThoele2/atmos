#pragma once

#include "LoadImageAssetResourceData.h"

namespace Atmos::Asset
{
    class ImageManager
    {
    public:
        virtual ~ImageManager() = 0;

        virtual Resource::Loaded<Resource::Image> Load(const File::Path& filePath) = 0;
        virtual Resource::Loaded<Resource::Image> Load(const Buffer& memory) = 0;
    };
}