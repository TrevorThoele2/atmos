#include "SDLWindow.h"

#include "WindowCreationFailed.h"

namespace Atmos::Window
{
    SDLWindow::SDLWindow(Logging::Logger& logger) : WindowBase(logger, "SDL")
    {
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
            throw WindowCreationFailed(String(SDL_GetError()));

        SDL_VERSION(&info.version);

        const auto center = Position();
        const auto size = ClientSize();

        underlying = SDL_CreateWindow("An Extraordinary Will", center.x, center.y, size.width, size.height, SDL_WINDOW_SHOWN);
        if (!underlying)
            throw WindowCreationFailed(String(SDL_GetError()));

        if (!SDL_GetWindowWMInfo(underlying, &info))
            throw WindowCreationFailed(String(SDL_GetError()));
    }

    SDLWindow::~SDLWindow()
    {
        Exit();
    }

    void SDLWindow::Show()
    {
        SDL_ShowWindow(underlying);
    }

    void SDLWindow::Exit()
    {
        SDL_DestroyWindow(underlying);
        SDL_Quit();
    }

    bool SDLWindow::IsCurrentlyFocused() const
    {
        const auto flags = SDL_GetWindowFlags(underlying);
        return (flags & SDL_WINDOW_INPUT_FOCUS) != 0;
    }

    void SDLWindow::Suspend(const Time::Duration<>& time)
    {
        const auto sleepFor = static_cast<DWORD>(std::chrono::duration_cast<Time::Milliseconds>(time).count());
        SDL_Delay(sleepFor);
    }

    bool SDLWindow::OnStartFrame()
    {
        SDL_Event event;

        while (SDL_PollEvent(&event) != 0)
        {
            if (event.type == SDL_QUIT)
                return false;
        }

        return true;
    }

    void* SDLWindow::Handle() const
    {
        return info.info.win.window;
    }

    void SDLWindow::OnPositionChanged()
    {
        const auto center = Position();
        SDL_SetWindowPosition(underlying, center.x, center.y);
    }

    void SDLWindow::OnSizeChanged()
    {
        const auto size = ClientSize();
        SDL_SetWindowSize(underlying, size.width, size.height);
    }

    void SDLWindow::OnFullscreenChanged()
    {
        if (IsFullscreen())
            SDL_SetWindowFullscreen(underlying, SDL_WINDOW_FULLSCREEN);
        else
            SDL_SetWindowFullscreen(underlying, 0);
    }

    auto SDLWindow::WindowSizeFromClientSize() const -> Size
    {
        int top = 0;
        int left = 0;
        int bottom = 0;
        int right = 0;
        SDL_GetWindowBordersSize(underlying, &top, &left, &bottom, &right);

        const auto clientSize = ClientSize();

        using Coordinate = Size::Dimension;
        return Size{ clientSize.width + left + right, clientSize.height + top + bottom };
    }

    auto SDLWindow::TotalScreenSize() const -> Size
    {
        SDL_DisplayMode displayMode;
        SDL_GetCurrentDisplayMode(0, &displayMode);
        return Size{ displayMode.w, displayMode.h };
    }
}