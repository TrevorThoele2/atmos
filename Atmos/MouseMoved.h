#pragma once

#include <Arca/Signal.h>

#include "ScreenPoint.h"

namespace Atmos::Input
{
    struct MouseMoved
    {
        Spatial::ScreenPoint previous;
        Spatial::ScreenPoint current;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Input::MouseMoved>
    {
        static const ObjectType objectType = ObjectType::Signal;
        static TypeName TypeName() { return "Atmos::Input::MouseMoved"; }
    };
}