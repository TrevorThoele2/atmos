#pragma once

#include <Arca/Command.h>
#include "CreateAssetResource.h"
#include "ImageAssetResource.h"
#include "Buffer.h"
#include "Name.h"

namespace Atmos::Asset::Resource
{
    template<>
    struct Create<Image>
    {
        DataBuffer buffer;
        Name name;
        ImageSize size;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Asset::Resource::Create<Atmos::Asset::Resource::Image>>
    {
        static const ObjectType objectType = ObjectType::Command;
        static inline const TypeName typeName = "Atmos::Asset::Resource::Create<Atmos::Asset::Resource::Image>";
        using Result = std::unique_ptr<Atmos::Asset::Resource::Image>;
    };
}