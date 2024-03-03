#pragma once

#include "TimeValue.h"
#include "AxisBoundingBox2D.h"

#include "ScreenPosition.h"
#include "ScreenDimensions.h"

namespace Atmos
{
    class ObjectManager;
    class LoggingSystem;
    class GraphicsSystem;

    class WindowBase
    {
    public:
        typedef ScreenPosition Position;
        typedef ScreenDimensions Size;
    public:
        virtual ~WindowBase() = 0;

        virtual void Setup() = 0;

        virtual void Show() = 0;
        virtual void Exit() = 0;
        virtual bool IsCurrentlyFocused() const = 0;
        virtual void Suspend(const TimeValue& time) = 0;
        virtual bool OnStartFrame() = 0;

        void ToggleFullscreen();
        void SetFullscreen(bool set = true);
        bool IsFullscreen() const;
        bool IsWindowed() const;

        Size DefaultSize();
        Size ClientSize();
        Size WindowSize();
        Position StartPosition();
    protected:
        WindowBase(ObjectManager& objectManager);
    protected:
        void SetWindowDimensions();

        virtual AxisBoundingBox2D AdjustWindowDimensions() = 0;
        virtual void OnSetWindowDimensions() = 0;
        virtual Position GetDefaultWindowPosition() = 0;
        virtual void OnSetFullscreen() = 0;
    protected:
        ObjectManager* GetObjectManager();
        const ObjectManager* GetObjectManager() const;
        LoggingSystem* FindLoggingSystem();
    private:
        Size clientSize;
        Size windowSize;
        Position startPosition;
    private:
        bool isFocusLost;
        bool isFullscreen;
    private:
        ObjectManager* objectManager;
        GraphicsSystem* FindGraphicsSystem();
    };
}