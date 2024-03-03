#pragma once

#include "LoadAssetResourceData.h"
#include "ImageAssetResource.h"
#include "ImageAssetFileType.h"

namespace Atmos::Asset::Resource
{
    template<>
    struct LoadedData<Image>
    {
        Buffer buffer;
        ImageFileType fileType;
        ImageSize size;
    };
}