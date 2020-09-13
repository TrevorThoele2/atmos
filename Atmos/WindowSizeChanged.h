#pragma once

#include <Arca/Signal.h>

#include "ScreenSize.h"

namespace Atmos::Window
{
    struct SizeChanged
    {
        const Spatial::ScreenSize size;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Window::SizeChanged>
    {
        static const ObjectType objectType = ObjectType::Signal;
        static inline const TypeName typeName = "Atmos::Window::SizeChanged";
    };
}