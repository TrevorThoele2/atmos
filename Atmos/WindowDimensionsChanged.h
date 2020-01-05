#pragma once

#include <Arca/Signal.h>

#include "ScreenDimensions.h"

namespace Atmos::Window
{
    struct DimensionsChanged
    {
        const ScreenDimensions dimensions;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Window::DimensionsChanged>
    {
        static const ObjectType objectType = ObjectType::Signal;
        static inline const TypeName typeName = "WindowDimensionsChanged";
    };
}