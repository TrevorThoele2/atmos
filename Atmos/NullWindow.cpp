#include "NullWindow.h"

namespace Atmos::Window
{
    NullWindow::NullWindow(Logging::Logger& logger) : WindowBase(logger, "Null")
    {}

    void NullWindow::Show()
    {}

    void NullWindow::Exit()
    {}

    bool NullWindow::IsCurrentlyFocused() const
    {
        return true;
    }

    void NullWindow::Suspend(const Time::Milliseconds& time)
    {}

    bool NullWindow::OnStartFrame()
    {
        return true;
    }

    void* NullWindow::Handle() const
    {
        return nullptr;
    }

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