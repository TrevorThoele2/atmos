#include "Surface.h"

namespace Atmos::Render
{
    Surface::Data::~Data()
    {}

    Surface* Surface::Data::GetOwner() const
    {
        return owner;
    }

    Surface::Surface(DataPtr&& data) : size(0, 0)
    {
        SetData(std::move(data));
        FitToWindow();
    }

    Surface::Surface(Surface&& arg) : size(std::move(arg.size))
    {
        SetData(std::move(arg.data));
    }

    Surface& Surface::operator=(Surface&& arg)
    {
        SetData(std::move(arg.data));
        size = std::move(arg.size);
        return *this;
    }

    Surface::Data* Surface::GetData() const
    {
        return data.get();
    }

    void Surface::Present()
    {
        data->Present();
    }

    void Surface::Reset()
    {
        data->Reset();
        SetupDimensions();
    }

    void Surface::Release()
    {
        data->Release();
    }

    void Surface::FitToWindow()
    {
        if (size == data->GetSize())
            return;

        Release();
        Reset();
    }

    const Surface::Size& Surface::GetSize() const
    {
        return size;
    }

    void Surface::SetData(DataPtr&& set)
    {
        data = std::move(set);
        data->owner = this;
    }

    void Surface::SetupDimensions()
    {
        size = std::move(data->GetSize());
    }

    void Surface::SetAsRenderTargetImpl()
    {
        data->SetAsRenderTarget();
    }
}