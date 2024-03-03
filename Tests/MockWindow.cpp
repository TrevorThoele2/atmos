#include "MockWindow.h"

void MockWindow::Show()
{}

void MockWindow::Exit()
{}

bool MockWindow::IsCurrentlyFocused() const
{
    return true;
}

void MockWindow::Suspend(const Time::Duration<>& time)
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

String MockWindow::TypeName() const
{
    return "Mock";
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