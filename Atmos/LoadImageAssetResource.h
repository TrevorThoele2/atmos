#pragma once

#include <Arca/Command.h>
#include "LoadAssetResource.h"
#include "ImageAssetResource.h"
#include "FilePath.h"

#include "Buffer.h"
#include "ImageAssetType.h"

namespace Atmos::Asset::Resource
{
    template<>
    struct LoadFromFile<Image>
    {
        File::Path filePath;
    };

    template<>
    struct LoadFromMemory<Image>
    {
        DataBuffer memory;
    };

    template<>
    struct Loaded<Image>
    {
        DataBuffer buffer;
        ImageType type;
        ImageSize size;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Asset::Resource::LoadFromFile<Atmos::Asset::Resource::Image>>
    {
        static const ObjectType objectType = ObjectType::Command;
        static inline const TypeName typeName = "Atmos::Asset::Resource::LoadFromFile<Atmos::Asset::Resource::Image>";
        using Result = Atmos::Asset::Resource::Loaded<Atmos::Asset::Resource::Image>;
    };

    template<>
    struct Traits<Atmos::Asset::Resource::LoadFromMemory<Atmos::Asset::Resource::Image>>
    {
        static const ObjectType objectType = ObjectType::Command;
        static inline const TypeName typeName = "Atmos::Asset::Resource::LoadFromMemory<Atmos::Asset::Resource::Image>";
        using Result = Atmos::Asset::Resource::Loaded<Atmos::Asset::Resource::Image>;
    };
}