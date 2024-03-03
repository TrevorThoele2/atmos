#pragma once

#include <Arca/Command.h>
#include "LoadAssetResourceData.h"
#include "ShaderAssetResource.h"
#include "FilePath.h"

#include "Buffer.h"

namespace Atmos::Asset::Resource
{
    template<>
    struct LoadDataFromFile<Shader>
    {
        File::Path filePath;
    };

    template<>
    struct LoadDataFromMemory<Shader>
    {
        Buffer memory;
    };

    template<>
    struct Loaded<Shader>
    {
        Buffer data;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Asset::Resource::LoadDataFromFile<Atmos::Asset::Resource::Shader>>
    {
        static const ObjectType objectType = ObjectType::Command;
        static inline const TypeName typeName =
            "Atmos::Asset::Resource::LoadDataFromFile<Atmos::Asset::Resource::Shader>";
        using Result = Atmos::Asset::Resource::Loaded<Atmos::Asset::Resource::Shader>;
    };

    template<>
    struct Traits<Atmos::Asset::Resource::LoadDataFromMemory<Atmos::Asset::Resource::Shader>>
    {
        static const ObjectType objectType = ObjectType::Command;
        static inline const TypeName typeName =
            "Atmos::Asset::Resource::LoadDataFromMemory<Atmos::Asset::Resource::Shader>";
        using Result = Atmos::Asset::Resource::Loaded<Atmos::Asset::Resource::Shader>;
    };
}