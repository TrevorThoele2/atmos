#pragma once

#include "UniqueProvider.h"
#include <AGUI/Engine.h>

namespace Atmos
{
    using GuiEngineProvider = UniqueProvider<Agui::Engine>;
}

namespace Arca
{
    template<>
    struct Traits<Atmos::GuiEngineProvider>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static const TypeName typeName;
        static const Locality locality = Locality::Global;
    };
}