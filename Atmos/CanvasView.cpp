#include "CanvasView.h"

namespace Atmos::Render
{
    CanvasView::CanvasView(Init init, Arca::RelicIndex<Canvas> source) :
        ClosedTypedRelic(init), source(std::move(source)), bounds(init.id, init.owner)
    {}

    const Canvas& CanvasView::Source() const
    {
        return *source;
    }

    const Bounds& CanvasView::Bounds() const
    {
        return *bounds;
    }
}