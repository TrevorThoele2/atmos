#pragma once

#include "AssetCurator.h"
#include "AudioAsset.h"

namespace Atmos::Asset
{
    using AudioAssetCurator = AssetCurator<AudioAsset>;

    template<>
    struct AssetCuratorTraits<AudioAsset> : AssetCuratorTraitsBase<AudioAsset>
    {
        constexpr static DebugStatisticsSize debugStatisticsSize = &Debug::Statistics::Memory::audioAssetSize;
    };
}