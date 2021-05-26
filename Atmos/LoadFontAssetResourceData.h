#pragma once

#include "LoadAssetResourceData.h"
#include "FontAssetResource.h"

namespace Atmos::Asset::Resource
{
    template<>
    struct LoadedData<Font>
    {
        Buffer buffer;
    };
}