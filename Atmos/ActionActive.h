#pragma once

#include <Arca/Signal.h>

#include "ActionAsset.h"

namespace Atmos::Input
{
    struct ActionActive
    {
        Arca::Index<Asset::Action> action;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Input::ActionActive>
    {
        static constexpr ObjectType objectType = ObjectType::Signal;
        static const inline TypeName typeName = "Atmos::Input::ActionActive";
    };
}