#pragma once

#include <Arca/Command.h>
#include "LoadAsset.h"
#include "FilePath.h"

#include "Buffer.h"
#include "ImageAssetType.h"

namespace Atmos::Asset
{
    template<>
    struct Load<Image>
    {
        File::Path filePath;
    };

    template<>
    struct Loaded<Image>
    {
        Buffer buffer;
        ImageType type;
        ImageSize size;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Asset::Load<Atmos::Asset::Image>>
    {
        static const ObjectType objectType = ObjectType::Command;
        static inline const TypeName typeName = "LoadImageAsset";
        using Result = Atmos::Asset::Loaded<Atmos::Asset::Image>;
    };
}