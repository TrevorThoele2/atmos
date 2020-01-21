#pragma once

#include <optional>

#include "MappedAssets.h"
#include "FileAsset.h"

#include <Arca/Reliquary.h>

namespace Atmos::Asset
{
    template<class T, std::enable_if_t<std::is_base_of_v<FileAsset, T>, int> = 0>
    bool ShouldCreateAsset(Arca::Reliquary& reliquary, const File::Name& fileName)
    {
        const auto mappedAssets = Arca::GlobalIndex<MappedAssets<T>>(reliquary);
        return !mappedAssets->Exists(fileName);
    }

    template<class T, std::enable_if_t<!std::is_base_of_v<FileAsset, T>, int> = 0>
    bool ShouldCreateAsset(Arca::Reliquary& reliquary, const Name& name)
    {
        const auto mappedAssets = Arca::GlobalIndex<MappedAssets<T>>(reliquary);
        return !mappedAssets->Exists(name);
    }
}