#pragma once

#include <Arca/Command.h>
#include "ShaderAssetData.h"
#include "Buffer.h"
#include "Name.h"

namespace Atmos::Asset
{
    struct CreateShaderAssetData
    {
        Buffer buffer;
        Name name;
        String entryPoint;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Asset::CreateShaderAssetData>
    {
        static const ObjectType objectType = ObjectType::Command;
        static inline const TypeName typeName = "CreateShaderAssetData";
        using Result = std::unique_ptr<Atmos::Asset::ShaderAssetData>;
    };
}