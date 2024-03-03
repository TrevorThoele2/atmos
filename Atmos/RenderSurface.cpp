
#include "RenderSurface.h"

#include "Environment.h"

namespace Atmos
{
    RenderSurface* RenderSurface::Data::GetOwner() const
    {
        return owner;
    }

    void RenderSurface::SetData(Data *set)
    {
        data.reset(set);
        data->owner = this;
    }

    void RenderSurface::SetData(std::unique_ptr<Data> &&set)
    {
        data = std::move(set);
        data->owner = this;
    }

    void RenderSurface::SetupDimensions()
    {
        dimensions = std::move(data->GetDimensions());
    }

    void RenderSurface::SetAsRenderTargetImpl()
    {
        data->SetAsRenderTarget();
    }

    RenderSurface::RenderSurface(Data *data) : dimensions(0, 0)
    {
        SetData(data);
        FitToWindow();
    }

    RenderSurface::RenderSurface(RenderSurface &&arg) : dimensions(std::move(arg.dimensions))
    {
        SetData(std::move(arg.data));
    }

    RenderSurface& RenderSurface::operator=(RenderSurface &&arg)
    {
        SetData(std::move(arg.data));
        dimensions = std::move(arg.dimensions);
        return *this;
    }

    RenderSurface::Data* RenderSurface::GetData() const
    {
        return data.get();
    }

    void RenderSurface::DestroyThis()
    {
        Environment::GetGraphics()->DestroyRenderSurface(*this);
    }

    void RenderSurface::SetAsRenderTarget()
    {
        Environment::GetGraphics()->SetRenderTarget(*this);
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
        if (dimensions == data->GetDimensions())
            return;

        Release();
        Reset();
    }

    const RenderSurface::ScreenDimensions& RenderSurface::GetDimensions() const
    {
        return dimensions;
    }
}