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
        static constexpr ObjectType objectType = ObjectType::Command;
        static const inline TypeName typeName = "Atmos::Asset::Resource::Create<Atmos::Asset::Resource::Font>";
        using Result = std::unique_ptr<Atmos::Asset::Resource::Font>;
    };
}