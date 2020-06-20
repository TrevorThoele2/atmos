#pragma once

#include <Arca/Signal.h>

#include "Action.h"

namespace Atmos::Input
{
    struct ActionActive
    {
        const Action& action;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Input::ActionActive>
    {
        static const ObjectType objectType = ObjectType::Signal;
        static inline const TypeName typeName = "Atmos::Input::ActionActive";
    };
}