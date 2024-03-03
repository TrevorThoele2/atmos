#pragma once

#include "TimeValue.h"

#include "ScreenPoint.h"
#include "ScreenSize.h"

namespace Atmos::Window
{
    class WindowBase
    {
    public:
        using Point = Spatial::ScreenPoint;
        using Size = Spatial::ScreenSize;
    public:
        virtual ~WindowBase() = 0;

        void Setup();

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
        WindowBase() = default;
        virtual void SetupImpl() = 0;

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
    };
}