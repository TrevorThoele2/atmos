#pragma once

#include "MappedAssets.h"

namespace Atmos::Asset
{
    template<class T>
    bool ShouldCreate(Arca::Reliquary& reliquary, const Name& name)
    {
        const auto mappedAssets = reliquary.Find<Mapped<T>>();
        return !mappedAssets->Exists(name);
    }
}