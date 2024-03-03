#pragma once

#include "AssetCurator.h"
#include "MaterialAsset.h"

namespace Atmos::Asset
{
    using MaterialAssetCurator = AssetCurator<MaterialAsset>;

    template<>
    struct AssetCuratorTraits<MaterialAsset> : AssetCuratorTraitsBase<MaterialAsset>
    {
        constexpr static DebugStatisticsSize debugStatisticsSize = &Debug::Statistics::Memory::materialAssetSize;
    };
}