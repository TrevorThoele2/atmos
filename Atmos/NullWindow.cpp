#include "NullWindow.h"

namespace Atmos::Window
{
    void NullWindow::Show()
    {}

    void NullWindow::Exit()
    {}

    bool NullWindow::IsCurrentlyFocused() const
    {
        return true;
    }

    void NullWindow::Suspend(const Time::Duration<>& time)
    {}

    bool NullWindow::OnStartFrame()
    {
        return true;
    }

    void* NullWindow::Handle() const
    {
        return nullptr;
    }

    void NullWindow::SetupImpl()
    {}

    Spatial::AxisAlignedBox2D NullWindow::AdjustWindowDimensions()
    {
        return Spatial::AxisAlignedBox2D();
    }

    void NullWindow::OnSetWindowDimensions()
    {}

    NullWindow::Position NullWindow::GetDefaultWindowPosition()
    {
        return Position();
    }

    void NullWindow::OnSetFullscreen()
    {}
}