#include "WindowsWindow.h"

#include "WindowCreationFailed.h"

#include "SpatialAlgorithms.h"
#include <tchar.h>

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    // Sort through and find what code to run for the message given
    switch (message)
    {
        // This message is read when the window is closed
    case WM_DESTROY:
        // Close the application entirely
        PostQuitMessage(0);
        return 0;
    default:
        break;
    }

    // Handle any messages the switch statement didn't
    return DefWindowProc(hWnd, message, wParam, lParam);
}

namespace Atmos::Window
{
    WindowsWindow::WindowsWindow(HINSTANCE hInstance, int nCmdShow, const String& className) :
        nCmdShow(nCmdShow), className(className)
    {
        windowClass.lpszClassName = className.c_str();
        windowClass.hInstance = hInstance;
        windowClass.style = CS_HREDRAW | CS_VREDRAW;
        windowClass.lpfnWndProc = WindowProc;
        windowClass.cbSize = sizeof(WNDCLASSEX);

        const auto registered = RegisterClassEx(&windowClass);
        if (registered == 0)
            throw WindowCreationFailed(LastErrorMessage());

        ChangeStyles();

        const auto center = Position();
        const auto size = ClientSize();

        hwnd = CreateWindowEx(
            currentExStyle,
            windowClass.lpszClassName,
            windowClass.lpszClassName,
            currentStyle,
            center.x,
            center.y,
            size.width,
            size.height,
            nullptr,
            nullptr,
            windowClass.hInstance,
            nullptr);

        if (hwnd == nullptr)
            throw WindowCreationFailed(LastErrorMessage());
    }

    void WindowsWindow::Show()
    {
        ShowWindow(hwnd, nCmdShow);
    }

    void WindowsWindow::Exit()
    {
        PostQuitMessage(0);
    }

    bool WindowsWindow::IsCurrentlyFocused() const
    {
        return GetFocus() == hwnd;
    }

    void WindowsWindow::Suspend(const Time::Duration<>& time)
    {
        const auto sleepFor = static_cast<DWORD>(std::chrono::duration_cast<Time::Milliseconds>(time).count());
        Sleep(sleepFor);
    }

    bool WindowsWindow::OnStartFrame()
    {
        MSG msg;
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                return false;

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        return true;
    }

    void* WindowsWindow::Handle() const
    {
        return hwnd;
    }

    auto WindowsWindow::WindowSizeFromClientSize() const -> Size
    {
        const auto clientSize = ClientSize();

        RECT rectangle;
        SetRect(&rectangle, 0, 0, clientSize.width, clientSize.height);
        AdjustWindowRectEx(&rectangle, currentStyle, false, currentExStyle);

        using Coordinate = Spatial::AxisAlignedBox2D::Coordinate;
        return Size{ rectangle.right - rectangle.left, rectangle.bottom - rectangle.top };
    }

    auto WindowsWindow::TotalScreenSize() const -> Size
    {
        return Size(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
    }

    void WindowsWindow::OnPositionChanged()
    {
        if (IsWindowed())
            SetWindowPositionAndSize(HWND_NOTOPMOST);
    }

    void WindowsWindow::OnSizeChanged()
    {
        if (IsWindowed())
            SetWindowPositionAndSize(HWND_NOTOPMOST);
    }

    void WindowsWindow::OnFullscreenChanged()
    {
        ChangeStyles();
        if (IsFullscreen())
            SetWindowPositionAndSize(HWND_TOPMOST);
        else
            SetWindowSize();

        SetWindowLongPtr(hwnd, GWL_STYLE, currentStyle);
        SetWindowLongPtr(hwnd, GWL_EXSTYLE, currentExStyle);
        Show();
    }

    void WindowsWindow::ChangeStyles()
    {
        if (IsFullscreen())
        {
            currentStyle = fullscreenStyle;
            currentExStyle = fullscreenStyleEx;
        }
        else
        {
            currentStyle = windowedStyle;
            currentExStyle = windowedStyleEx;
        }
    }

    void WindowsWindow::SetWindowPositionAndSize(HWND after)
    {
        const auto center = Position();
        const auto size = ClientSize();

        SetWindowPos(
            hwnd,
            after,
            center.x,
            center.y,
            size.width,
            size.height,
            SWP_SHOWWINDOW);
    }

    String WindowsWindow::LastErrorMessage()
    {
        const auto error = GetLastError();
        LPVOID messageBuffer;
        FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            nullptr,
            error,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            reinterpret_cast<LPTSTR>(&messageBuffer),
            0,
            nullptr);
        const auto displayBuffer = static_cast<LPCTSTR>(messageBuffer);

        const String errorMessage = _T(displayBuffer);

        LocalFree(messageBuffer);

        return errorMessage;
    }
}