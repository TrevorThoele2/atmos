#pragma once

#include <Arca/SignalTraits.h>

#include "Action.h"

namespace Atmos::Input
{
    struct ActionPressed
    {
        const Action& action;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Input::ActionPressed>
    {
        static const ObjectType objectType = ObjectType::Signal;
    };
}