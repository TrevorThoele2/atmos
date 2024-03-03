#include "WindowBase.h"

#include <Arca/Reliquary.h>

#include "WindowDimensionsChanged.h"
#include "ReconstructGraphics.h"

namespace Atmos::Window
{
    WindowBase::~WindowBase() = default;

    void WindowBase::Setup(Arca::Reliquary& reliquary)
    {
        this->reliquary = &reliquary;
        SetupImpl();
    }

    void WindowBase::ToggleFullscreen()
    {
        SetFullscreen(!IsFullscreen());
    }

    void WindowBase::SetFullscreen(bool set)
    {
        if (IsFullscreen() == set)
            return;

        isFullscreen = set;
        OnSetFullscreen();
        SetFullscreen(isFullscreen);
        reliquary->Raise<Render::ReconstructGraphics>();
    }

    bool WindowBase::IsFullscreen() const
    {
        return isFullscreen;
    }

    bool WindowBase::IsWindowed() const
    {
        return !isFullscreen;
    }

    WindowBase::Size WindowBase::DefaultSize() const
    {
        return Size(1024, 768);
    }

    WindowBase::Size WindowBase::ClientSize() const
    {
        return clientSize;
    }

    WindowBase::Size WindowBase::WindowSize() const
    {
        return windowSize;
    }

    WindowBase::Position WindowBase::StartPosition() const
    {
        return startPosition;
    }

    void WindowBase::SetWindowDimensions()
    {
        // Set lengths
        {
            const auto box = AdjustWindowDimensions();
            windowSize.width = static_cast<Size::Dimension>(abs(box.Left()) + abs(box.Right()));
            windowSize.height = static_cast<Size::Dimension>(abs(box.Top()) + abs(box.Bottom()));
        }

        // Set default position
        {
            const auto defaultPosition = GetDefaultWindowPosition();
            startPosition = defaultPosition;
        }

        reliquary->Raise<DimensionsChanged>(clientSize);
        OnSetWindowDimensions();
    }

    Arca::Reliquary& WindowBase::Reliquary()
    {
        return *reliquary;
    }

    const Arca::Reliquary& WindowBase::Reliquary() const
    {
        return *reliquary;
    }
}