#pragma once

#include <Arca/Command.h>
#include "CreateAssetResource.h"
#include "FontAssetResource.h"
#include "Buffer.h"

namespace Atmos::Asset::Resource
{
    template<>
    struct Create<Font>
    {
        Buffer buffer;
        String name;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Asset::Resource::Create<Atmos::Asset::Resource::Font>>
    {
        static const ObjectType objectType = ObjectType::Command;
        static TypeName TypeName() { return "Atmos::Asset::Resource::Create<Atmos::Asset::Resource::Font>"; }
        using Result = std::unique_ptr<Atmos::Asset::Resource::Font>;
    };
}