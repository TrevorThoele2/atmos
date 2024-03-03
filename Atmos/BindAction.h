#pragma once

#include <Arca/Command.h>

#include "ActionAsset.h"

namespace Atmos::Input
{
    struct BindAction
    {
        Arca::Index<Asset::Action> action;
        Key key;
        std::set<Key> modifiers;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Input::BindAction>
    {
        static const ObjectType objectType = ObjectType::Command;
        static TypeName TypeName() { return "Atmos::Input::BindAction"; }
    };
}