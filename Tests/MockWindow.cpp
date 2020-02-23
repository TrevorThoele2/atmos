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

void MockWindow::SetupImpl()
{}

AxisAlignedBox2D MockWindow::AdjustWindowDimensions()
{
    return {};
}

void MockWindow::OnSetWindowDimensions()
{}

auto MockWindow::GetDefaultWindowPosition() -> Position
{
    return {};
}

void MockWindow::OnSetFullscreen()
{}