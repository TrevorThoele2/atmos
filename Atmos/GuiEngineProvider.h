#pragma once

#include "UniqueProviderRelic.h"
#include <AGUI/Engine.h>

namespace Atmos
{
    using GuiEngineProvider = UniqueProviderRelic<Agui::Engine>;
}

namespace Arca
{
    template<>
    struct Traits<Atmos::GuiEngineProvider>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static inline const TypeName typeName = "Atmos::GuiEngineProvider";
        static const Locality locality = Locality::Global;
    };
}