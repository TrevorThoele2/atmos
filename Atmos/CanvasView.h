#pragma once

#include <Arca/ClosedTypedRelicAutomation.h>
#include "Canvas.h"
#include "Bounds.h"

namespace Atmos::Render
{
    class CanvasView final : public Arca::ClosedTypedRelicAutomation<CanvasView>
    {
    public:
        void PostConstruct();
        void Initialize(Canvas& source);

        [[nodiscard]] const Canvas& Source() const;

        [[nodiscard]] const Bounds& Bounds() const;
    private:
        Canvas* source = nullptr;
        Arca::ShardIndex<Atmos::Bounds> bounds;
    };
}

namespace Arca
{
    template<>
    struct Traits<::Atmos::Render::CanvasView>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static inline const TypeName typeName = "CanvasView";
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Render::CanvasView, BinaryArchive> final :
        public ArcaNullScribe<::Atmos::Render::CanvasView, BinaryArchive>
    {};
}