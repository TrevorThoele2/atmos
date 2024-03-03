
#include "WindowsWindow.h"

#include "LoggingSystem.h"

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
    }

    // Handle any messages the switch statement didn't
    return DefWindowProc(hWnd, message, wParam, lParam);
}

namespace Atmos
{
    WindowsWindowBase::WindowsWindowBase(ObjectManager& objectManager) : WindowBase(objectManager)
    {}

    WindowsWindow::WindowsWindow(ObjectManager& objectManager, HINSTANCE hInstance, LPSTR lpCmdLine, int nCmdShow, const String& className) :
        WindowsWindowBase(objectManager),
        nCmdShow(nCmdShow), className(className + "Client"),
        windowedStyle(WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_OVERLAPPED), windowedStyleEx(0),
        fullscreenStyle(WS_POPUP), fullscreenStyleEx(WS_EX_TOPMOST)
    {}

    void WindowsWindow::Setup()
    {
        if (IsWindowed())
        {
            style = windowedStyle;
            exStyle = windowedStyleEx;
        }
        else
        {
            style = fullscreenStyle;
            exStyle = fullscreenStyleEx;
        }

        // Create the window and use the result as the handle
        hwnd = CreateWindowEx(
            exStyle,
            wc.lpszClassName,
            wc.lpszClassName,
            style,
            StartPosition().x,
            StartPosition().y,
            Size().width,
            Size().height,
            nullptr,
            nullptr,
            wc.hInstance,
            nullptr);

        if (hwnd == nullptr)
            FindLoggingSystem()->Log("Window creation failed.",
                LogType::ERROR_SEVERE);
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

    void WindowsWindow::Suspend(const TimeValue &time)
    {
        auto test = static_cast<unsigned int>(time.ConvertValue(TimeValueEpoch::MILLISECONDS));
        Sleep(test);
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

    HWND WindowsWindow::GetHwnd()
    {
        return hwnd;
    }

    AxisBoundingBox2D WindowsWindow::AdjustWindowDimensions()
    {
        RECT rect;
        SetRect(&rect, 0, 0, ClientSize().width, ClientSize().height);
        AdjustWindowRectEx(&rect, style, false, exStyle);

        typedef AxisBoundingBox2D::Coordinate Coordinate;
        return AxisBoundingBox2D(
            static_cast<Coordinate>(rect.left),
            static_cast<Coordinate>(rect.top),
            static_cast<Coordinate>(rect.right),
            static_cast<Coordinate>(rect.bottom));
    }

    void WindowsWindow::OnSetWindowDimensions()
    {
        if (IsWindowed())
            SetWindowPos(HWND_NOTOPMOST);
    }

    WindowsWindow::Position WindowsWindow::GetDefaultWindowPosition()
    {
        return Position(
            static_cast<Position::Dimension>(GetSystemMetrics(SM_CXSCREEN)),
            static_cast<Position::Dimension>(GetSystemMetrics(SM_CYSCREEN)));
    }

    void WindowsWindow::OnSetFullscreen()
    {
        if (IsFullscreen())
        {
            style = fullscreenStyle;
            exStyle = fullscreenStyleEx;
            SetWindowPos(HWND_TOPMOST);
        }
        else
        {
            style = windowedStyle;
            exStyle = windowedStyleEx;
            SetWindowDimensions();
        }

        SetWindowLongPtr(hwnd, GWL_STYLE, style);
        SetWindowLongPtr(hwnd, GWL_EXSTYLE, exStyle);
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