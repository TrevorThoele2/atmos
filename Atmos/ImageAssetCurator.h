#pragma once

#include "AssetCurator.h"
#include "ImageAsset.h"

namespace Atmos::Asset
{
    using ImageAssetCurator = AssetCurator<ImageAsset>;

    template<>
    struct AssetCuratorTraits<ImageAsset> : AssetCuratorTraitsBase<ImageAsset>
    {
        constexpr static DebugStatisticsSize debugStatisticsSize = &Debug::Statistics::Memory::imageAssetSize;
    };
}