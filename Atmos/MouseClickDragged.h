#pragma once

#include <Arca/SignalTraits.h>

#include "MousePosition.h"

namespace Atmos::Input
{
    struct MouseMoved
    {
        const MousePositionDelta delta;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Input::MouseMoved>
    {
        static const ObjectType objectType = ObjectType::Signal;
    };
}