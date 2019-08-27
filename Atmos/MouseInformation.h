#pragma once

#include <Arca/ClosedTypedRelicAutomation.h>

#include "ScreenPosition.h"

namespace Atmos::Input
{
    class MouseInformation final : public Arca::ClosedTypedRelicAutomation<MouseInformation>
    {
    public:
        ScreenPosition position;
        ScreenPosition previousMousePosition;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Input::MouseInformation>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static const TypeName typeName;
        static const Locality locality = Locality::Global;
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Input::MouseInformation, BinaryArchive> final
        : public ArcaNullScribe<Atmos::Input::MouseInformation, BinaryArchive>
    {};
}