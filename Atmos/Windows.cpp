
#include "Windows.h"

#include "WindowsInput.h"
#include "DXGraphics.h"
#include "DXAudio.h"
#include "FileSystemWindows.h"

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    // Sort through and find what code to run for the message given
    switch(message)
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
    void Windows::SetupImpl()
    {
        if (GetFlag(Flag::WINDOWED))
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
        hWnd = CreateWindowEx(exStyle, wc.lpszClassName, wc.lpszClassName, style, GetDimensionX().GetStart(), GetDimensionY().GetStart(), GetDimensionX().GetTotalLength(), GetDimensionY().GetTotalLength(), nullptr, nullptr, wc.hInstance, nullptr);
        if (hWnd == nullptr)
        {
            Logger::Log("Window creation failed.",
                Logger::Type::ERROR_SEVERE);
        }
    }

    AxisBoundingBox2D Windows::AdjustWindowDimensions()
    {
        RECT rect;
        SetRect(&rect, 0, 0, GetClientWidth(), GetClientHeight());
        AdjustWindowRectEx(&rect, style, false, exStyle);

        typedef AxisBoundingBox2D::Coordinate Coordinate;
        return AxisBoundingBox2D(static_cast<Coordinate>(rect.left), static_cast<Coordinate>(rect.top), static_cast<Coordinate>(rect.right), static_cast<Coordinate>(rect.bottom));
    }

    void Windows::OnSetWindowDimensions()
    {
        if(GetFlag(Flag::WINDOWED))
            SetWindowPos(HWND_NOTOPMOST);
    }

    Position2D Windows::GetDefaultWindowPosition()
    {
        return Position2D(static_cast<Position2D::ValueT>(GetSystemMetrics(SM_CXSCREEN)), static_cast<Position2D::ValueT>(GetSystemMetrics(SM_CYSCREEN)));
    }

    void Windows::OnSetFullscreen()
    {
        if (!GetFlag(Flag::WINDOWED))
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

        SetWindowLongPtr(hWnd, GWL_STYLE, style);
        SetWindowLongPtr(hWnd, GWL_EXSTYLE, exStyle);
        Show();
    }

    void Windows::SetWindowPos(HWND after)
    {
        ::SetWindowPos(hWnd, after, GetDimensionX().GetStart(), GetDimensionY().GetStart(), GetDimensionX().GetTotalLength(), GetDimensionY().GetTotalLength(), SWP_SHOWWINDOW);
    }

    Windows::Windows(HINSTANCE hInstance, LPSTR lpCmdLine, int nCmdShow, const String &className) : nCmdShow(nCmdShow), windowedStyle(WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_OVERLAPPED), windowedStyleEx(0), fullscreenStyle(WS_POPUP), fullscreenStyleEx(WS_EX_TOPMOST), className(className + "Client")
    {
        ZeroMemory(&wc, sizeof(WNDCLASSEX));
        wc.hInstance = hInstance;
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc = WindowProc;
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wc.lpszClassName = this->className.c_str();
        wc.hIcon = static_cast<HICON>(LoadImage(nullptr, "logo.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE));

        if (!RegisterClassEx(&wc))
        {
            Logger::Log("Class registration failed.",
                Logger::Type::ERROR_SEVERE,
                Logger::NameValueVector{ NameValuePair("Code", static_cast<unsigned int>(GetLastError())) });
        }
    }

    void Windows::Show()
    {
        // Display the window on the screen
        ShowWindow(hWnd, nCmdShow);
    }

    bool Windows::StartFrame()
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

    void Windows::Exit()
    {
        PostQuitMessage(0);
    }

    bool Windows::IsFocused() const
    {
        return GetFocus() == hWnd;
    }

    void Windows::CreateInput()
    {
        Environment::CreateInputHandler<Input::WindowsHandler>();
    }

    void Windows::CreateGraphics()
    {
        Environment::CreateGraphicsHandler<DX9GraphicsHandler>(hWnd, Join2<UINT>(GetClientWidth(), GetClientHeight()), !GetFlag(Flag::WINDOWED));
    }

    void Windows::CreateAudio()
    {
        Environment::CreateAudioHandler<DX9AudioHandler>();
    }

    void Windows::CreateFileSystem()
    {
        Environment::CreateFileSystem<FileSystem::WindowsHandler>();
    }

    HWND Windows::GetHwnd()
    {
        return hWnd;
    }
}