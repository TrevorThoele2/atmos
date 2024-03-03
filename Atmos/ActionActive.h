#pragma once

#include <Arca/SignalTraits.h>

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
    };
}