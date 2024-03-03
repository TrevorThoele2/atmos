#pragma once

#include <Arca/ClosedTypedRelic.h>

#include "ScreenPoint.h"

namespace Atmos::Input
{
    class MouseInformation final : public Arca::ClosedTypedRelic<MouseInformation>
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
        static inline const TypeName typeName = "Atmos::Input::MouseInformation";
        static const Locality locality = Locality::Global;
    };
}

namespace Inscription
{
    template<class Archive>
    struct ScribeTraits<Atmos::Input::MouseInformation, Archive> final
    {
        using Category = ArcaNullScribeCategory<Atmos::Input::MouseInformation>;
    };
}