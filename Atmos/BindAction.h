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
        static constexpr ObjectType objectType = ObjectType::Command;
        static const inline TypeName typeName = "Atmos::Input::BindAction";
    };
}