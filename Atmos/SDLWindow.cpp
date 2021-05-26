#include "SDLWindow.h"

#include "WindowCreationFailed.h"

namespace Atmos::Window
{
    SDLWindow::SDLWindow(Logging::Logger& logger) : WindowBase(logger, "SDL")
    {
        SDL_VERSION(&info.version)

        const auto center = Position();
        const auto size = ClientSize();

        underlying = SDL_CreateWindow(
            "An Extraordinary Will",
            static_cast<int>(center.x),
            static_cast<int>(center.y),
            static_cast<int>(size.width),
            static_cast<int>(size.height),
            SDL_WINDOW_SHOWN);
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
        SDL_SetWindowPosition(underlying, static_cast<int>(center.x), static_cast<int>(center.y));
    }

    void SDLWindow::OnSizeChanged()
    {
        const auto size = ClientSize();
        SDL_SetWindowSize(underlying, static_cast<int>(size.width), static_cast<int>(size.height));
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
        return Size
        {
            clientSize.width + static_cast<Point::Value>(left) + static_cast<Point::Value>(right),
            clientSize.height + static_cast<Point::Value>(top) + static_cast<Point::Value>(bottom) };
    }

    auto SDLWindow::TotalScreenSize() const -> Size
    {
        SDL_DisplayMode displayMode;
        SDL_GetCurrentDisplayMode(0, &displayMode);
        return Size{ static_cast<Point::Value>(displayMode.w), static_cast<Point::Value>(displayMode.h) };
    }
}