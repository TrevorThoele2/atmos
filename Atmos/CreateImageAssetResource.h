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
        Buffer buffer;
        Name name;
        Spatial::Size2D size;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Asset::Resource::Create<Atmos::Asset::Resource::Image>>
    {
        static constexpr ObjectType objectType = ObjectType::Command;
        static const inline TypeName typeName = "Atmos::Asset::Resource::Create<Atmos::Asset::Resource::Image>";
        using Result = std::unique_ptr<Atmos::Asset::Resource::Image>;
    };
}