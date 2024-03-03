#pragma once

#include <Arca/Command.h>

#include "ActionAsset.h"

namespace Atmos::Input
{
    struct BindAction
    {
        Arca::Index<Asset::Action> action;
        std::set<Key> keys;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Input::BindAction>
    {
        static const ObjectType objectType = ObjectType::Command;
        static inline const TypeName typeName = "Atmos::Input::BindAction";
    };
}