#pragma once

#include <Arca/Command.h>
#include "FilePath.h"

#include "Buffer.h"
#include "ImageAssetType.h"
#include "Size2D.h"

namespace Atmos::Asset
{
    struct LoadImageAsset
    {
        File::Path filePath;
    };

    struct LoadedImageAsset
    {
        Buffer buffer;
        ImageAssetType type;
        Size2D size;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Asset::LoadImageAsset>
    {
        static const ObjectType objectType = ObjectType::Command;
        static inline const TypeName typeName = "LoadImageAsset";
        using Result = Atmos::Asset::LoadedImageAsset;
    };
}