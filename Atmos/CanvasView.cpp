#include "CanvasView.h"

namespace Atmos::Render
{
    void CanvasView::PostConstruct(ShardTuple shards)
    {
        bounds = std::get<0>(shards);
    }

    void CanvasView::Initialize(Canvas& source)
    {
        this->source = &source;
    }

    const Canvas& CanvasView::Source() const
    {
        return *source;
    }

    const Bounds& CanvasView::Bounds() const
    {
        return *bounds;
    }
}

namespace Arca
{
    const TypeName Traits<::Atmos::Render::CanvasView>::typeName = "CanvasView";
}