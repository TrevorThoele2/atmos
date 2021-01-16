#pragma once

#include "LoadAssetResourceData.h"
#include "ShaderAssetResource.h"

namespace Atmos::Asset::Resource
{
    template<>
    struct LoadedData<Shader>
    {
        Buffer data;
    };
}