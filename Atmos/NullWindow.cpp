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

    void NullWindow::OnPositionChanged()
    {}

    void NullWindow::OnSizeChanged()
    {}

    void NullWindow::OnFullscreenChanged()
    {}

    auto NullWindow::WindowSizeFromClientSize() const -> Size
    {
        return Size();
    }

    auto NullWindow::TotalScreenSize() const -> Size
    {
        return Size();
    }
}