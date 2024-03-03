#pragma once

#include <Arca/Signal.h>

#include "String.h"

namespace Atmos::Input
{
    struct TextEntered
    {
        String text;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Input::TextEntered>
    {
        static constexpr ObjectType objectType = ObjectType::Signal;
        static const inline TypeName typeName = "Atmos::Input::TextEntered";
    };
}