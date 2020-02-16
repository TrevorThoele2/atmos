#pragma once

#include <Arca/Command.h>
#include "ScreenSize.h"

namespace Atmos::Render
{
    struct ResizeCamera
    {
        ScreenSize toSize;

        explicit ResizeCamera(const ScreenSize& toSize) : toSize(toSize)
        {}
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::ResizeCamera>
    {
        static const ObjectType objectType = ObjectType::Command;
        static inline const TypeName typeName = "ResizeCamera";
    };
}