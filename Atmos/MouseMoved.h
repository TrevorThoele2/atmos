#pragma once

#include <Arca/Signal.h>

#include "Point2D.h"

namespace Atmos::Input
{
    struct MouseMoved
    {
        Spatial::Point2D previous;
        Spatial::Point2D current;
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