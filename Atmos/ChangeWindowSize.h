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
        static constexpr ObjectType objectType = ObjectType::Command;
        static const inline TypeName typeName = "Atmos::Window::ChangeSize";
    };
}