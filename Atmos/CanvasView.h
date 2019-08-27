#pragma once

#include <Arca/ClosedTypedRelicAutomation.h>
#include "Canvas.h"
#include "Bounds.h"

namespace Atmos::Render
{
    class CanvasView final : public Arca::ClosedTypedRelicAutomation<CanvasView, Bounds>
    {
    public:
        void PostConstruct(ShardTuple shards);
        void Initialize(Canvas& source);

        [[nodiscard]] const Canvas& Source() const;

        [[nodiscard]] const Bounds& Bounds() const;
    private:
        Canvas* source = nullptr;
        Atmos::Bounds* bounds = nullptr;
    };
}

namespace Arca
{
    template<>
    struct Traits<::Atmos::Render::CanvasView>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static const TypeName typeName;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Render::CanvasView, BinaryArchive> final :
        public ArcaNullScribe<::Atmos::Render::CanvasView, BinaryArchive>
    {};
}