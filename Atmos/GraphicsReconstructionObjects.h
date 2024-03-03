#pragma once

#include "ScreenSize.h"

namespace Atmos::Asset
{
    class Shader;
}

namespace Atmos::Render
{
    class MainSurface;
    class AncillarySurface;

    struct GraphicsReconstructionObjects
    {
        ScreenSize screenSize;
        std::vector<Asset::Shader*> shaderAssets;
        MainSurface* mainSurface;
        std::vector<AncillarySurface*> ancillarySurfaces;
    };
}