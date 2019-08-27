#pragma once

#include <Arca/ClosedTypedRelicAutomation.h>
#include "UniqueProvider.h"

#include <AGUI/Engine.h>

namespace Atmos
{
    class GuiEngineProvider final
        : public Arca::ClosedTypedRelicAutomation<GuiEngineProvider>
        , public UniqueProvider<Agui::Engine>
    {
    public:
        void PostConstruct();
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::GuiEngineProvider>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static const TypeName typeName;
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::GuiEngineProvider, BinaryArchive> final
        : public ArcaNullScribe<Atmos::GuiEngineProvider, BinaryArchive>
    {};
}