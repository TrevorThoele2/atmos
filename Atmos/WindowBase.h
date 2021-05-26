#pragma once

#include "TimePoint.h"

#include "Point2D.h"
#include "Size2D.h"

#include "String.h"

#include "Logger.h"

namespace Atmos::Window
{
    class WindowBase
    {
    public:
        using Point = Spatial::Point2D;
        using Size = Spatial::Size2D;
    public:
        virtual ~WindowBase() = 0;

        virtual void Show() = 0;
        virtual void Exit() = 0;
        [[nodiscard]] virtual bool IsCurrentlyFocused() const = 0;
        virtual void Suspend(const Time::Duration<>& time) = 0;
        virtual bool OnStartFrame() = 0;

        void SetFullscreen(bool set);
        [[nodiscard]] bool IsFullscreen() const;
        [[nodiscard]] bool IsWindowed() const;

        void CenterOnScreen();

        void ChangeSize(Size clientSize);
        [[nodiscard]] Size ClientSize() const;
        [[nodiscard]] Size WindowSize() const;
        [[nodiscard]] Point Position() const;

        [[nodiscard]] virtual void* Handle() const = 0;
    protected:
        WindowBase(Logging::Logger& logger, String typeName);

        void SetWindowSize();

        virtual void OnPositionChanged() = 0;
        virtual void OnSizeChanged() = 0;
        virtual void OnFullscreenChanged() = 0;
        [[nodiscard]] virtual Size WindowSizeFromClientSize() const = 0;

        [[nodiscard]] virtual Size TotalScreenSize() const = 0;
    private:
        Size clientSize;
        Size windowSize;
        Point position;
    private:
        bool isFullscreen = false;
    private:
        String typeName;
        Logging::Logger* logger;
    };
}