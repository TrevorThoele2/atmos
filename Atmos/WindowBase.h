#pragma once

#include "TimeValue.h"
#include "AxisAlignedBox2D.h"

#include "ScreenPosition.h"
#include "ScreenDimensions.h"

namespace Arca
{
    class Reliquary;
}

namespace Atmos::Window
{
    class WindowBase
    {
    public:
        using Position = ScreenPosition;
        using Size = ScreenDimensions;
    public:
        virtual ~WindowBase() = 0;

        void Setup(Arca::Reliquary& reliquary);

        virtual void Show() = 0;
        virtual void Exit() = 0;
        [[nodiscard]] virtual bool IsCurrentlyFocused() const = 0;
        virtual void Suspend(const Time::Value& time) = 0;
        virtual bool OnStartFrame() = 0;

        void ToggleFullscreen();
        void SetFullscreen(bool set = true);
        [[nodiscard]] bool IsFullscreen() const;
        [[nodiscard]] bool IsWindowed() const;

        [[nodiscard]] Size DefaultSize() const;
        [[nodiscard]] Size ClientSize() const;
        [[nodiscard]] Size WindowSize() const;
        [[nodiscard]] Position StartPosition() const;
    protected:
        virtual void SetupImpl() = 0;

        void SetWindowDimensions();

        virtual AxisAlignedBox2D AdjustWindowDimensions() = 0;
        virtual void OnSetWindowDimensions() = 0;
        virtual Position GetDefaultWindowPosition() = 0;
        virtual void OnSetFullscreen() = 0;
    protected:
        [[nodiscard]] Arca::Reliquary& Reliquary();
        [[nodiscard]] const Arca::Reliquary& Reliquary() const;
    private:
        Size clientSize;
        Size windowSize;
        Position startPosition;
    private:
        bool isFullscreen = false;
    private:
        Arca::Reliquary* reliquary = nullptr;
    };
}