#pragma once

#include "TimeValue.h"
#include "AxisAlignedBox2D.h"

#include "ScreenPoint.h"
#include "ScreenSize.h"

namespace Atmos::Window
{
    class WindowBase
    {
    public:
        using Position = Spatial::ScreenPoint;
        using Size = Spatial::ScreenSize;
    public:
        virtual ~WindowBase() = 0;

        void Setup();

        virtual void Show() = 0;
        virtual void Exit() = 0;
        [[nodiscard]] virtual bool IsCurrentlyFocused() const = 0;
        virtual void Suspend(const Time::Duration<>& time) = 0;
        virtual bool OnStartFrame() = 0;

        void ToggleFullscreen();
        void SetFullscreen(bool set = true);
        [[nodiscard]] bool IsFullscreen() const;
        [[nodiscard]] bool IsWindowed() const;

        [[nodiscard]] Size DefaultSize() const;
        [[nodiscard]] Size ClientSize() const;
        [[nodiscard]] Size WindowSize() const;
        [[nodiscard]] Position StartPosition() const;

        [[nodiscard]] virtual [[nodiscard]] void* Handle() const = 0;
    protected:
        virtual void SetupImpl() = 0;

        void SetWindowDimensions();

        virtual Spatial::AxisAlignedBox2D AdjustWindowDimensions() = 0;
        virtual void OnSetWindowDimensions() = 0;
        virtual Position GetDefaultWindowPosition() = 0;
        virtual void OnSetFullscreen() = 0;
    private:
        Size clientSize;
        Size windowSize;
        Position startPosition;
    private:
        bool isFullscreen = false;
    };
}