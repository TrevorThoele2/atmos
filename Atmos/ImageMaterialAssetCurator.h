#pragma once

#include "AssetCurator.h"
#include "ImageMaterialAsset.h"

namespace Atmos::Asset
{
    using ImageMaterialCurator = Curator<ImageMaterial>;

    template<>
    struct CuratorTraits<ImageMaterial> : CuratorTraitsBase<ImageMaterial>
    {
        constexpr static DebugStatisticsSize debugStatisticsSize = &Debug::Statistics::Memory::materialAssetSize;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Asset::ImageMaterialCurator>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static inline const TypeName typeName = "Atmos::Asset::ImageMaterialCurator";
        using HandledCommands = HandledCommands<Atmos::Asset::FindByName<Atmos::Asset::ImageMaterial>>;
    };
}

namespace Inscription
{
    template<class Archive>
    struct ScribeTraits<Atmos::Asset::ImageMaterialCurator, Archive> final
    {
        using Category = AssetScribeCategory<Atmos::Asset::ImageMaterialCurator>;
    };
}