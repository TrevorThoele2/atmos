#include "Surface.h"

namespace Atmos::Render
{
    Surface::Data::~Data() = default;

    Surface* Surface::Data::Owner() const
    {
        return owner;
    }

    Surface::Surface(DataPtr&& data) : size(0, 0)
    {
        SetData(std::move(data));
        FitToWindow();
    }

    Surface::Surface(Surface&& arg) noexcept : size(arg.size)
    {
        SetData(std::move(arg.data));
    }

    Surface& Surface::operator=(Surface&& arg) noexcept
    {
        SetData(std::move(arg.data));
        size = arg.size;
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
        if (size == data->Size())
            return;

        Release();
        Reset();
    }

    ScreenDimensions Surface::Size() const
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
        size = data->Size();
    }

    void Surface::SetAsRenderTargetImpl()
    {
        data->SetAsRenderTarget();
    }
}