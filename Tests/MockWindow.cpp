#include "MockWindow.h"

MockWindow::MockWindow(Logging::Logger& logger) : WindowBase(logger, "Mock")
{}

void MockWindow::Show()
{}

void MockWindow::Exit()
{}

bool MockWindow::IsCurrentlyFocused() const
{
    return true;
}

void MockWindow::Suspend(const Time::Milliseconds& time)
{}

bool MockWindow::OnStartFrame()
{
    if (hasRan)
        return false;
    hasRan = true;
    return true;
}

void* MockWindow::Handle() const
{
    return nullptr;
}

void MockWindow::OnPositionChanged()
{}

void MockWindow::OnSizeChanged()
{}

void MockWindow::OnFullscreenChanged()
{}

auto MockWindow::WindowSizeFromClientSize() const -> Size
{
    return Size{};
}

auto MockWindow::TotalScreenSize() const -> Size
{
    return Size{};
}