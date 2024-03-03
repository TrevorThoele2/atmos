#pragma once

#include "ScreenSize.h"

namespace Atmos::Asset
{
    class ShaderAsset;
}

namespace Atmos::Render
{
    class MainSurface;
    class AncillarySurface;

    struct GraphicsReconstructionObjects
    {
        ScreenSize screenSize;
        std::vector<Asset::ShaderAsset*> shaderAssets;
        MainSurface* mainSurface;
        std::vector<AncillarySurface*> ancillarySurfaces;
    };
}