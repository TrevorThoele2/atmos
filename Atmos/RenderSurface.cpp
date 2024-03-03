
#include "RenderSurface.h"

namespace Atmos
{
    RenderSurface::Data::~Data()
    {}

    RenderSurface* RenderSurface::Data::GetOwner() const
    {
        return owner;
    }

    RenderSurface::RenderSurface(DataPtr&& data) : size(0, 0)
    {
        SetData(std::move(data));
        FitToWindow();
    }

    RenderSurface::RenderSurface(RenderSurface&& arg) : size(std::move(arg.size))
    {
        SetData(std::move(arg.data));
    }

    RenderSurface& RenderSurface::operator=(RenderSurface&& arg)
    {
        SetData(std::move(arg.data));
        size = std::move(arg.size);
        return *this;
    }

    RenderSurface::Data* RenderSurface::GetData() const
    {
        return data.get();
    }

    void RenderSurface::Present()
    {
        data->Present();
    }

    void RenderSurface::Reset()
    {
        data->Reset();
        SetupDimensions();
    }

    void RenderSurface::Release()
    {
        data->Release();
    }

    void RenderSurface::FitToWindow()
    {
        if (size == data->GetSize())
            return;

        Release();
        Reset();
    }

    const RenderSurface::Size& RenderSurface::GetSize() const
    {
        return size;
    }

    void RenderSurface::SetData(DataPtr&& set)
    {
        data = std::move(set);
        data->owner = this;
    }

    void RenderSurface::SetupDimensions()
    {
        size = std::move(data->GetSize());
    }

    void RenderSurface::SetAsRenderTargetImpl()
    {
        data->SetAsRenderTarget();
    }
}