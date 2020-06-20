#pragma once

#include <Arca/Signal.h>

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
        static inline const TypeName typeName = "Atmos::Input::ActionPressed";
    };
}