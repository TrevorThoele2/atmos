#pragma once

#include <Arca/Command.h>
#include "CreateAssetData.h"
#include "ImageAssetData.h"
#include "Buffer.h"
#include "Name.h"

namespace Atmos::Asset
{
    template<>
    struct CreateData<ImageData>
    {
        Buffer buffer;
        Name name;
        ImageSize size;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Asset::CreateData<Atmos::Asset::ImageData>>
    {
        static const ObjectType objectType = ObjectType::Command;
        static inline const TypeName typeName = "CreateAssetData<ImageAssetData>";
        using Result = std::unique_ptr<Atmos::Asset::ImageData>;
    };
}