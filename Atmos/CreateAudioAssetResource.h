#pragma once

#include <Arca/Command.h>
#include "CreateAssetResource.h"
#include "AudioAssetResource.h"
#include "Buffer.h"
#include "Name.h"

namespace Atmos::Asset::Resource
{
    template<>
    struct Create<Audio>
    {
        Buffer buffer;
        Name name;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Asset::Resource::Create<Atmos::Asset::Resource::Audio>>
    {
        static const ObjectType objectType = ObjectType::Command;
        static TypeName TypeName() { return "Atmos::Asset::Resource::Create<Atmos::Asset::Resource::Audio>"; }
        using Result = std::unique_ptr<Atmos::Asset::Resource::Audio>;
    };
}