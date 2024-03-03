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
        static constexpr ObjectType objectType = ObjectType::Signal;
        static const inline TypeName typeName = "Atmos::FocusLost";
    };
}