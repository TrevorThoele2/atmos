#pragma once

#include <Arca/ClosedTypedRelic.h>
#include "Canvas.h"
#include "Bounds.h"

namespace Atmos::Render
{
    class CanvasView final : public Arca::ClosedTypedRelic<CanvasView>
    {
    public:
        CanvasView(Init init, Arca::RelicIndex<Canvas> source);

        [[nodiscard]] const Canvas& Source() const;

        [[nodiscard]] const Bounds& Bounds() const;
    private:
        Arca::RelicIndex<Canvas> source;
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