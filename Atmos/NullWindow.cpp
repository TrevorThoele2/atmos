#include "NullWindow.h"

namespace Atmos::Window
{
    NullWindow::NullWindow(ObjectManager& objectManager) : WindowBase(objectManager)
    {}

    void NullWindow::Setup()
    {}

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