#pragma once

#include <Arca/SignalTraits.h>

namespace Atmos
{
    class FocusLost
    {};
}

namespace Arca
{
    template<>
    struct Traits<Atmos::FocusLost>
    {
        static const ObjectType objectType = ObjectType::Signal;
        static const TypeName typeName;
    };
}