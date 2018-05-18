
#include "CanvasView.h"

#include "Environment.h"

namespace Atmos
{
    void CanvasView::CalculateBounds()
    {
        SetWidth(static_cast<float>(canvas->GetWidth()));
        SetHeight(static_cast<float>(canvas->GetHeight()));
    }

    void CanvasView::DrawImpl() const
    {
        Environment::GetGraphics()->RenderCanvasView(*this);
    }

    Position3D::ValueT CanvasView::GetZHeight() const
    {
        return 0;
    }

    CanvasView::CanvasView() : RenderFragment(true), canvas(nullptr)
    {}

    CanvasView::CanvasView(const Canvas &canvas) : RenderFragment(true), canvas(&canvas)
    {
        CalculateBounds();
    }

    CanvasView::CanvasView(const CanvasView &arg) : RenderFragment(arg), canvas(arg.canvas)
    {}

    CanvasView::CanvasView(CanvasView &&arg) : RenderFragment(std::move(arg)), canvas(arg.canvas)
    {}

    CanvasView& CanvasView::operator=(const CanvasView &arg)
    {
        RenderFragment::operator=(arg);
        canvas = arg.canvas;
        return *this;
    }

    CanvasView& CanvasView::operator=(CanvasView &&arg)
    {
        RenderFragment::operator=(std::move(arg));
        canvas = arg.canvas;
        return *this;
    }

    bool CanvasView::operator==(const CanvasView &arg) const
    {
        return RenderFragment::operator==(arg) && canvas == arg.canvas;
    }

    bool CanvasView::operator!=(const CanvasView &arg) const
    {
        return !(*this == arg);
    }

    void CanvasView::SetCanvas(const Canvas &set)
    {
        canvas = &set;
        CalculateBounds();
    }

    const Canvas& CanvasView::GetCanvas() const
    {
        return *canvas;
    }
}