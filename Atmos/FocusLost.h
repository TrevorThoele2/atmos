#pragma once

#include <Arca/Signal.h>

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
        static inline const TypeName typeName = "FocusLost";
    };
}