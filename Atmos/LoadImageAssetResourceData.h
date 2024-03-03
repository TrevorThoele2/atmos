#pragma once

#include <Arca/Command.h>
#include "LoadAssetResourceData.h"
#include "ImageAssetResource.h"
#include "FilePath.h"

#include "Buffer.h"
#include "ImageAssetFileType.h"

namespace Atmos::Asset::Resource
{
    template<>
    struct LoadDataFromFile<Image>
    {
        File::Path filePath;
    };

    template<>
    struct LoadDataFromMemory<Image>
    {
        DataBuffer memory;
    };

    template<>
    struct Loaded<Image>
    {
        DataBuffer buffer;
        ImageFileType fileType;
        ImageSize size;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Asset::Resource::LoadDataFromFile<Atmos::Asset::Resource::Image>>
    {
        static const ObjectType objectType = ObjectType::Command;
        static inline const TypeName typeName =
            "Atmos::Asset::Resource::LoadDataFromFile<Atmos::Asset::Resource::Image>";
        using Result = Atmos::Asset::Resource::Loaded<Atmos::Asset::Resource::Image>;
    };

    template<>
    struct Traits<Atmos::Asset::Resource::LoadDataFromMemory<Atmos::Asset::Resource::Image>>
    {
        static const ObjectType objectType = ObjectType::Command;
        static inline const TypeName typeName =
            "Atmos::Asset::Resource::LoadDataFromMemory<Atmos::Asset::Resource::Image>";
        using Result = Atmos::Asset::Resource::Loaded<Atmos::Asset::Resource::Image>;
    };
}