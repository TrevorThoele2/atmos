#pragma once

#include <Arca/SignalTraits.h>

namespace Atmos
{
    class FocusRegained
    {};
}

namespace Arca
{
    template<>
    struct Traits<Atmos::FocusRegained>
    {
        static const ObjectType objectType = ObjectType::Signal;
        static const TypeName typeName;
    };
}