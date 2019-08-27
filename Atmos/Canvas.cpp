#include "Canvas.h"

namespace Atmos::Render
{
    Canvas::Data::~Data() = default;

    Canvas* Canvas::Data::Owner() const
    {
        return owner;
    }

    Canvas::Canvas(DataPtr&& data, DimensionValue width, DimensionValue height) :
        width(width), height(height)
    {
        SetData(std::move(data));
    }

    Canvas::Canvas(Canvas&& arg) noexcept :
        isPainting(arg.isPainting), width(arg.width), height(arg.height)
    {
        SetData(std::move(arg.data));
    }

    Canvas& Canvas::operator=(Canvas&& arg) noexcept
    {
        width = arg.width;
        height = arg.height;
        isPainting = arg.isPainting;
        SetData(std::move(arg.data));
        return *this;
    }

    Canvas::Data* Canvas::GetData() const
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

    void Canvas::PaintPixel(const Position& position, const Color& color)
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

    Canvas::DimensionValue Canvas::Width() const
    {
        return width;
    }

    Canvas::DimensionValue Canvas::Height() const
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

    void Canvas::SetData(DataPtr&& set)
    {
        data = std::move(set);
        data->owner = this;
    }
}