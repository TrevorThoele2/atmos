
#include "WindowBase.h"

#include "ObjectManager.h"
#include "GraphicsSystem.h"
#include "LoggingSystem.h"

namespace Atmos
{
    WindowBase::~WindowBase()
    {}

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
        FindGraphicsSystem()->Get()->Reinitialize();
    }

    bool WindowBase::IsFullscreen() const
    {
        return isFullscreen;
    }

    bool WindowBase::IsWindowed() const
    {
        return !isFullscreen;
    }

    WindowBase::Size WindowBase::DefaultSize()
    {
        return Size(1024, 768);
    }

    WindowBase::Size WindowBase::ClientSize()
    {
        return clientSize;
    }

    WindowBase::Size WindowBase::WindowSize()
    {
        return windowSize;
    }

    WindowBase::Position WindowBase::StartPosition()
    {
        return startPosition;
    }

    WindowBase::WindowBase(ObjectManager& objectManager) : objectManager(&objectManager)
    {}

    void WindowBase::SetWindowDimensions()
    {
        // Set lengths
        {
            AxisBoundingBox2D box = AdjustWindowDimensions();
            windowSize.width = static_cast<Size::Dimension>(abs(box.GetLeft()) + abs(box.GetRight()));
            windowSize.height = static_cast<Size::Dimension>(abs(box.GetTop()) + abs(box.GetBottom()));
        }

        // Set default position
        {
            Position defaultPosition = GetDefaultWindowPosition();
            startPosition = defaultPosition;
        }

        auto graphicsManager = FindGraphicsSystem()->Get();
        if (graphicsManager)
            graphicsManager->SetMainDimensions(clientSize);
        OnSetWindowDimensions();
    }

    ObjectManager* WindowBase::GetObjectManager()
    {
        return objectManager;
    }

    const ObjectManager* WindowBase::GetObjectManager() const
    {
        return objectManager;
    }

    LoggingSystem* WindowBase::FindLoggingSystem()
    {
        return objectManager->FindSystem<LoggingSystem>();
    }

    GraphicsSystem* WindowBase::FindGraphicsSystem()
    {
        return objectManager->FindSystem<GraphicsSystem>();
    }
}