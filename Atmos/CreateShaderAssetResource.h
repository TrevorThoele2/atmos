#pragma once

#include <Arca/Command.h>
#include "CreateAssetResource.h"
#include "ShaderAssetResource.h"
#include "Buffer.h"
#include "Name.h"

namespace Atmos::Asset::Resource
{
    template<>
    struct Create<Shader>
    {
        Buffer buffer;
        Name name;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Asset::Resource::Create<Atmos::Asset::Resource::Shader>>
    {
        static const ObjectType objectType = ObjectType::Command;
        static inline const TypeName typeName = "CreateAssetData<ShaderAssetData>";
        using Result = std::unique_ptr<Atmos::Asset::Resource::Shader>;
    };
}