#pragma once

#include "MappedAssets.h"

namespace Atmos::Asset
{
    template<class T>
    bool ShouldCreateAsset(Arca::Reliquary& reliquary, const Name& name)
    {
        const auto mappedAssets = Arca::GlobalIndex<MappedAssets<T>>(reliquary);
        return !mappedAssets->Exists(name);
    }
}