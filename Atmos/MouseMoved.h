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
        static constexpr ObjectType objectType = ObjectType::Signal;
        static const inline TypeName typeName = "Atmos::Input::MouseMoved";
    };
}