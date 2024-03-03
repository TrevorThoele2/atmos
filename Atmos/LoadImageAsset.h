#pragma once

#include <Arca/Command.h>
#include "FilePath.h"

#include "Buffer.h"
#include "ImageAssetType.h"
#include "Size2D.h"

namespace Atmos::Asset
{
    struct LoadImage
    {
        File::Path filePath;
    };

    struct LoadedImage
    {
        Buffer buffer;
        ImageType type;
        ImageSize size;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Asset::LoadImage>
    {
        static const ObjectType objectType = ObjectType::Command;
        static inline const TypeName typeName = "LoadImageAsset";
        using Result = Atmos::Asset::LoadedImage;
    };
}