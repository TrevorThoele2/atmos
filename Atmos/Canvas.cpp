
#include "Canvas.h"

namespace Atmos
{
    Canvas* Canvas::Data::GetOwner() const
    {
        return owner;
    }

    void Canvas::SetData(Data *set)
    {
        data.reset(set);
        data->owner = this;
    }

    void Canvas::SetData(std::unique_ptr<Data> &&set)
    {
        data = std::move(set);
        data->owner = this;
    }

    Canvas::Canvas(Data *data, Dimension width, Dimension height) : painting(false), width(width), height(height)
    {
        SetData(data);
    }

    Canvas::Canvas(Canvas &&arg) : painting(std::move(arg.painting)), width(arg.width), height(arg.height)
    {
        SetData(std::move(arg.data));
    }

    Canvas& Canvas::operator=(Canvas &&arg)
    {
        painting = std::move(arg.painting);
        SetData(std::move(arg.data));
        width = arg.width;
        height = arg.height;
        return *this;
    }

    Canvas::Data* Canvas::GetData() const
    {
        return data.get();
    }

    void Canvas::StartPainting()
    {
        painting = true;
        data->StartPainting();
    }

    void Canvas::StopPainting()
    {
        if (!IsPainting())
            return;

        data->StopPainting();
        painting = false;
    }

    bool Canvas::IsPainting() const
    {
        return painting;
    }

    void Canvas::PaintPixel(const PositionT &position, const Color &color)
    {
        if (!IsPainting())
            return;

        data->PaintPixel(position, color, height);
    }

    void Canvas::Clear(const Color &color)
    {
        if (!IsPainting())
            return;

        data->Clear(color);
    }

    Canvas::Dimension Canvas::GetWidth() const
    {
        return width;
    }

    Canvas::Dimension Canvas::GetHeight() const
    {
        return height;
    }

    void Canvas::Release()
    {
        data->Release();
    }

    void Canvas::Reset()
    {
        data->Reset(width, height);
    }
}