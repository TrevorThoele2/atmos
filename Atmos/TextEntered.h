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
        static const ObjectType objectType = ObjectType::Signal;
        static TypeName TypeName() { return "Atmos::Input::TextEntered"; }
    };
}