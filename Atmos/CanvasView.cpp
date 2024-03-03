#include "CanvasView.h"

namespace Atmos::Render
{
    void CanvasView::PostConstruct()
    {
        bounds = Create<Atmos::Bounds>();
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