#pragma once

#include <Arca/Signal.h>

#include "ActionAsset.h"

namespace Atmos::Input
{
    struct ActionDepressed
    {
        Arca::Index<Asset::Action> action;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Input::ActionDepressed>
    {
        static const ObjectType objectType = ObjectType::Signal;
        static inline const TypeName typeName = "Atmos::Input::ActionDepressed";
    };
}