#pragma once

#include <Arca/SignalTraits.h>

#include "Action.h"

namespace Atmos::Input
{
    struct ActionDepressed
    {
        const Action& action;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Input::ActionDepressed>
    {
        static const ObjectType objectType = ObjectType::Signal;
    };
}