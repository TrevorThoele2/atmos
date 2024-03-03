#pragma once

#include <Arca/Command.h>
#include "ScreenSize.h"

namespace Atmos::Window
{
    struct ChangeSize
    {
        Spatial::ScreenSize size;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Window::ChangeSize>
    {
        static const ObjectType objectType = ObjectType::Command;
        static inline const TypeName typeName = "Atmos::Window::ChangeSize";
    };
}