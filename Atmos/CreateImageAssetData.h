#pragma once

#include <Arca/Command.h>
#include "ImageAssetData.h"
#include "Buffer.h"
#include "Name.h"
#include "Size2D.h"

namespace Atmos::Asset
{
    struct CreateImageAssetData
    {
        Buffer buffer;
        Name name;
        Size2D size;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Asset::CreateImageAssetData>
    {
        static const ObjectType objectType = ObjectType::Command;
        static inline const TypeName typeName = "CreateImageAssetData";
        using Result = std::unique_ptr<Atmos::Asset::ImageAssetData>;
    };
}