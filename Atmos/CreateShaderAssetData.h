#pragma once

#include <Arca/Command.h>
#include "CreateAssetData.h"
#include "ShaderAssetData.h"
#include "Buffer.h"
#include "Name.h"

namespace Atmos::Asset
{
    template<>
    struct CreateData<ShaderData>
    {
        Buffer buffer;
        Name name;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Asset::CreateData<Atmos::Asset::ShaderData>>
    {
        static const ObjectType objectType = ObjectType::Command;
        static inline const TypeName typeName = "CreateAssetData<ShaderAssetData>";
        using Result = std::unique_ptr<Atmos::Asset::ShaderData>;
    };
}