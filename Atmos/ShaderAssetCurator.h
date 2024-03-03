#pragma once

#include "AssetCurator.h"
#include "ShaderAsset.h"

namespace Atmos::Asset
{
    using ShaderAssetCurator = AssetCurator<ShaderAsset>;

    template<>
    struct AssetCuratorTraits<ShaderAsset> : AssetCuratorTraitsBase<ShaderAsset>
    {
        constexpr static DebugStatisticsSize debugStatisticsSize = &Debug::Statistics::Memory::shaderAssetSize;
    };
}