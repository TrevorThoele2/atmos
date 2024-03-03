#pragma once

#include <Arca/Signal.h>

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
        static inline const TypeName typeName = "Atmos::FocusRegained";
    };
}