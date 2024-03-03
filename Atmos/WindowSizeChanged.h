#pragma once

#include <Arca/Signal.h>

#include "Size2D.h"

namespace Atmos::Window
{
    struct SizeChanged
    {
        const Spatial::Size2D size;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Window::SizeChanged>
    {
        static const ObjectType objectType = ObjectType::Signal;
        static TypeName TypeName() { return "Atmos::Window::SizeChanged"; }
    };
}