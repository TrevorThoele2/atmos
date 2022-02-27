#pragma once

#include "ShaderAssetResource.h"

namespace Atmos::Render
{
    struct Shaders
    {
        const Asset::Resource::Shader* vertex = nullptr;
        const Asset::Resource::Shader* fragment = nullptr;
    };
}