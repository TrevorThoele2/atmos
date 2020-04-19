#pragma once

#include "ShaderAsset.h"

namespace Atmos::Render::Vulkan
{
    struct LineShaders
    {
        Arca::Index<Asset::ShaderAsset> vertex;
        Arca::Index<Asset::ShaderAsset> fragment;
    };
}