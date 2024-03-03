#pragma once

#include <Arca/Signal.h>

#include "ScreenPositionDelta.h"

namespace Atmos::Input
{
    struct MouseMoved
    {
        const ScreenPositionDelta delta;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Input::MouseMoved>
    {
        static const ObjectType objectType = ObjectType::Signal;
        static inline const TypeName typeName = "InputMouseMoved";
    };
}