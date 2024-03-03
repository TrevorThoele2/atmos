#pragma once

#include <Arca/Command.h>
#include "LoadAssetResourceData.h"
#include "ScriptAssetResource.h"
#include "FilePath.h"

#include "Buffer.h"

namespace Atmos::Asset::Resource
{
    template<>
    struct LoadDataFromFile<Script>
    {
        File::Path filePath;
    };

    template<>
    struct LoadDataFromMemory<Script>
    {
        Buffer memory;
    };

    template<>
    struct Loaded<Script>
    {
        Buffer data;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Asset::Resource::LoadDataFromFile<Atmos::Asset::Resource::Script>>
    {
        static const ObjectType objectType = ObjectType::Command;
        static TypeName TypeName()
        {
            return "Atmos::Asset::Resource::LoadDataFromFile<Atmos::Asset::Resource::Script>";
        }
        using Result = Atmos::Asset::Resource::Loaded<Atmos::Asset::Resource::Script>;
    };

    template<>
    struct Traits<Atmos::Asset::Resource::LoadDataFromMemory<Atmos::Asset::Resource::Script>>
    {
        static const ObjectType objectType = ObjectType::Command;
        static TypeName TypeName()
        {
            return "Atmos::Asset::Resource::LoadDataFromMemory<Atmos::Asset::Resource::Script>";
        }
        using Result = Atmos::Asset::Resource::Loaded<Atmos::Asset::Resource::Script>;
    };
}