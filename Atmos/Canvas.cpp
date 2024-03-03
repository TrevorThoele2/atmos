#include "Canvas.h"

namespace Atmos::Render
{
    Canvas::Canvas(Init init, ScreenSize size, DataPtr&& data) :
        ClosedTypedRelic(init), size(size), data(std::move(data))
    {}

    Canvas::Canvas(Canvas&& arg) noexcept :
        ClosedTypedRelic(std::move(arg)),
        isPainting(arg.isPainting), size(arg.size), data(std::move(arg.data))
    {}

    Canvas& Canvas::operator=(Canvas && arg) noexcept
    {
        isPainting = arg.isPainting;
        size = arg.size;
        data = std::move(arg.data);
        ClosedTypedRelic::operator=(std::move(arg));
        return *this;
    }

    auto Canvas::Data() const -> DataT*
    {
        return data.get();
    }

    void Canvas::StartPainting()
    {
        isPainting = true;
        data->StartPainting();
    }

    void Canvas::StopPainting()
    {
        if (!IsPainting())
            return;

        data->StopPainting();
        isPainting = false;
    }

    bool Canvas::IsPainting() const
    {
        return isPainting;
    }

    void Canvas::PaintPixel(const ScreenPosition& position, const Color& color, ScreenPosition::Value height)
    {
        if (!IsPainting())
            return;

        data->PaintPixel(position, color, height);
    }

    void Canvas::Clear(const Color& color)
    {
        if (!IsPainting())
            return;

        data->Clear(color);
    }

    ScreenSize Canvas::Size() const
    {
        return size;
    }

    void Canvas::Release()
    {
        data->Release();
    }

    void Canvas::Reset()
    {
        data->Reset(size);
    }
}