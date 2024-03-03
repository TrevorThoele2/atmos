#pragma once

#include "LoadAssetResourceData.h"
#include "ScriptAssetResource.h"

namespace Atmos::Asset::Resource
{
    template<>
    struct LoadedData<Script>
    {
        Buffer data;
    };
}