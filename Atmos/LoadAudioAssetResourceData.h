#pragma once

#include "LoadAssetResourceData.h"
#include "AudioAssetResource.h"

namespace Atmos::Asset::Resource
{
    template<>
    struct LoadedData<Audio>
    {
        Buffer buffer;
    };
}