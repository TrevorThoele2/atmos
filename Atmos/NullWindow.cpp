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

    void NullWindow::Suspend(const Time::Value& time)
    {}

    bool NullWindow::OnStartFrame()
    {
        return true;
    }

    void NullWindow::SetupImpl()
    {}

    AxisAlignedBox2D NullWindow::AdjustWindowDimensions()
    {
        return AxisAlignedBox2D();
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