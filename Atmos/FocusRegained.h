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
        static constexpr ObjectType objectType = ObjectType::Signal;
        static const inline TypeName typeName = "Atmos::FocusRegained";
    };
}