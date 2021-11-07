#pragma once

#include <Arca/Signal.h>

#include <Arca/RelicID.h>
#include "Color.h"

namespace Atmos::Render
{
    struct ColorChanged
    {
        Arca::RelicID id;

        Color from;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::ColorChanged>
    {
        static constexpr ObjectType objectType = ObjectType::Signal;
        static const inline TypeName typeName = "Atmos::Render::ColorChanged";
    };
}