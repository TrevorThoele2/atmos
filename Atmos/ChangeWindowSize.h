#pragma once

#include <Arca/Command.h>
#include "Size2D.h"

namespace Atmos::Window
{
    struct ChangeSize
    {
        Spatial::Size2D size;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Window::ChangeSize>
    {
        static const ObjectType objectType = ObjectType::Command;
        static TypeName TypeName() { return "Atmos::Window::ChangeSize"; }
    };
}