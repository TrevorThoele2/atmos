#pragma once

#include "ScreenSize.h"

namespace Atmos::Asset
{
    class Shader;
}

namespace Atmos::Render
{
    class MainSurface;

    struct GraphicsReconstructionObjects
    {
        Spatial::ScreenSize screenSize;
        std::vector<Asset::Shader*> shaderAssets;
        MainSurface* mainSurface;
    };
}