#pragma once

#include "UniqueProvider.h"
#include "GraphicsManager.h"

namespace Atmos::Render
{
    class GraphicsManagerProvider final
        : public Arca::ClosedTypedRelicAutomation<GraphicsManagerProvider>
        , public UniqueProvider<GraphicsManager>
    {
    public:
        GraphicsManagerProvider() = default;
    public:
        void PostConstruct();
    protected:
        void OnChange() override;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::GraphicsManagerProvider>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static const TypeName typeName;
        static const Locality locality = Locality::Global;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Render::GraphicsManagerProvider, BinaryArchive> final :
        public ArcaNullScribe<::Atmos::Render::GraphicsManagerProvider, BinaryArchive>
    {};
}