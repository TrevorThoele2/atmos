#include "WindowsWindow.h"

#include "WindowCreationFailed.h"

#include "SpatialAlgorithms.h"

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
    WindowsWindow::WindowsWindow(int nCmdShow, const String& className) :
        nCmdShow(nCmdShow), className(className)
    {}

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

    void WindowsWindow::SetupImpl()
    {
        if (IsWindowed())
        {
            currentStyle = windowedStyle;
            currentExStyle = windowedStyleEx;
        }
        else
        {
            currentStyle = fullscreenStyle;
            currentExStyle = fullscreenStyleEx;
        }

        // Create the window and use the result as the handle
        hwnd = CreateWindowEx
        (
            currentExStyle,
            wc.lpszClassName,
            wc.lpszClassName,
            currentStyle,
            StartPosition().x,
            StartPosition().y,
            Size().width,
            Size().height,
            nullptr,
            nullptr,
            wc.hInstance,
            nullptr
        );

        if (hwnd == nullptr)
            throw WindowCreationFailed();
    }

    Spatial::AxisAlignedBox2D WindowsWindow::AdjustWindowDimensions()
    {
        RECT rect;
        SetRect(&rect, 0, 0, ClientSize().width, ClientSize().height);
        AdjustWindowRectEx(&rect, currentStyle, false, currentExStyle);

        using Coordinate = Spatial::AxisAlignedBox2D::Coordinate;
        return Spatial::ToAxisAlignedBox2D
        (
            static_cast<Coordinate>(rect.left),
            static_cast<Coordinate>(rect.top),
            static_cast<Coordinate>(rect.right),
            static_cast<Coordinate>(rect.bottom)
        );
    }

    void WindowsWindow::OnSetWindowDimensions()
    {
        if (IsWindowed())
            SetWindowPos(HWND_NOTOPMOST);
    }

    WindowsWindow::Position WindowsWindow::GetDefaultWindowPosition()
    {
        return Position(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
    }

    void WindowsWindow::OnSetFullscreen()
    {
        if (IsFullscreen())
        {
            currentStyle = fullscreenStyle;
            currentExStyle = fullscreenStyleEx;
            SetWindowPos(HWND_TOPMOST);
        }
        else
        {
            currentStyle = windowedStyle;
            currentExStyle = windowedStyleEx;
            SetWindowDimensions();
        }

        SetWindowLongPtr(hwnd, GWL_STYLE, currentStyle);
        SetWindowLongPtr(hwnd, GWL_EXSTYLE, currentExStyle);
        Show();
    }

    void WindowsWindow::SetWindowPos(HWND after)
    {
        ::SetWindowPos(
            hwnd,
            after,
            StartPosition().x,
            StartPosition().y,
            Size().width,
            Size().height,
            SWP_SHOWWINDOW);
    }
}