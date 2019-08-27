#pragma once

#include "AssetCurator.h"
#include "ScriptAsset.h"

namespace Atmos::Asset
{
    using ScriptAssetCurator = AssetCurator<ScriptAsset>;

    template<>
    struct AssetCuratorTraits<ScriptAsset> : AssetCuratorTraitsBase<ScriptAsset>
    {
        constexpr static DebugStatisticsSize debugStatisticsSize = &Debug::Statistics::Memory::scriptAssetSize;
    };
}