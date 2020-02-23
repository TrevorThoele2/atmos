#pragma once

#include <Arca/Command.h>
#include "FilePath.h"
#include "Buffer.h"

namespace Atmos::Asset
{
    struct ExtractAsset
    {
        File::Path filePath;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Asset::ExtractAsset>
    {
        static const ObjectType objectType = ObjectType::Command;
        static inline const TypeName typeName = "ExtractAsset";
        using Result = Atmos::Buffer;
    };
}