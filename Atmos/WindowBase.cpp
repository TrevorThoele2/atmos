#include "WindowBase.h"

namespace Atmos::Window
{
    WindowBase::~WindowBase()
    {
        logger->Log(
            "Destroyed window.",
            Logging::Severity::Information,
            Logging::Details{ {"Type Name", typeName} });
    }

    void WindowBase::SetFullscreen(bool set)
    {
        if (IsFullscreen() == set)
            return;

        isFullscreen = set;
        OnFullscreenChanged();
        SetFullscreen(isFullscreen);
    }

    bool WindowBase::IsFullscreen() const
    {
        return isFullscreen;
    }

    bool WindowBase::IsWindowed() const
    {
        return !isFullscreen;
    }

    void WindowBase::CenterOnScreen()
    {
        const auto totalScreenSize = TotalScreenSize();
        position.x = (totalScreenSize.width - windowSize.width) / 2;
        position.y = (totalScreenSize.height - windowSize.height) / 2;
        OnPositionChanged();
    }

    void WindowBase::ChangeSize(Size clientSize)
    {
        this->clientSize = clientSize;
        SetWindowSize();
    }

    WindowBase::Size WindowBase::ClientSize() const
    {
        return clientSize;
    }

    WindowBase::Size WindowBase::WindowSize() const
    {
        return windowSize;
    }

    WindowBase::Point WindowBase::Position() const
    {
        return position;
    }

    WindowBase::WindowBase(Logging::Logger& logger, String typeName) :
        typeName(typeName), logger(&logger)
    {
        this->logger->Log(
            "Created window.",
            Logging::Severity::Information,
            Logging::Details{ {"Type Name", typeName} });
    }

    void WindowBase::SetWindowSize()
    {
        const auto size = WindowSizeFromClientSize();
        windowSize.width = size.width;
        windowSize.height = size.height;

        OnSizeChanged();
    }
}