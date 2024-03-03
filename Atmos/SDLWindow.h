#pragma once

#include "WindowBase.h"
#include <SDL.h>
#include <SDL_syswm.h>

namespace Atmos::Window
{
    class SDLWindow final : public WindowBase
    {
    public:
        SDLWindow();
        ~SDLWindow();

        void Show() override;
        void Exit() override;
        [[nodiscard]] bool IsCurrentlyFocused() const override;
        void Suspend(const Time::Duration<>& time) override;
        bool OnStartFrame() override;

        [[nodiscard]] void* Handle() const override;

        [[nodiscard]] String TypeName() const override;
    protected:
        void OnPositionChanged() override;
        void OnSizeChanged() override;
        void OnFullscreenChanged() override;
        [[nodiscard]] Size WindowSizeFromClientSize() const override;

        [[nodiscard]] Size TotalScreenSize() const override;
    private:
        SDL_Window* underlying = nullptr;
        SDL_SysWMinfo info{};
    };
}