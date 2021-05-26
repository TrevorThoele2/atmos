#pragma once

#include "Size2D.h"

namespace Atmos::Asset
{
    class Shader;
}

namespace Atmos::Render
{
    class MainSurface;

    struct GraphicsReconstructionObjects
    {
        Spatial::Size2D screenSize;
        std::vector<Asset::Shader*> shaderAssets;
        MainSurface* mainSurface;
    };
}