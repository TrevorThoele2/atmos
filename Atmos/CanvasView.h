#pragma once

#include "RenderFragment.h"

namespace Atmos
{
    class Canvas;
    class CanvasView : public RenderFragment
    {
    private:
        const Canvas *canvas;
        void CalculateBounds();

        void DrawImpl() const override;
        Position3D::ValueT GetZHeight() const override;
    public:
        CanvasView();
        CanvasView(const Canvas &canvas);
        CanvasView(const CanvasView &arg);
        CanvasView(CanvasView &&arg);
        CanvasView& operator=(const CanvasView &arg);
        CanvasView& operator=(CanvasView &&arg);

        bool operator==(const CanvasView &arg) const;
        bool operator!=(const CanvasView &arg) const;

        void SetCanvas(const Canvas &set);
        const Canvas& GetCanvas() const;
    };
}