#pragma once

#include "WindowBase.h"
#include "WindowsInclude.h"

#include "TimeValue.h"
#include "String.h"

namespace Atmos::Window
{
    class WindowsWindow final : public WindowBase
    {
    public:
        WindowsWindow(int nCmdShow, const String& className);

        void Show() override;
        void Exit() override;
        [[nodiscard]] bool IsCurrentlyFocused() const override;
        void Suspend(const Time::Value& time) override;
        bool OnStartFrame() override;

        [[nodiscard]] HWND Hwnd() const;
    protected:
        void SetupImpl() override;

        AxisAlignedBox2D AdjustWindowDimensions() override;
        void OnSetWindowDimensions() override;
        Position GetDefaultWindowPosition() override;
        void OnSetFullscreen() override;
    private:
        HWND hwnd = nullptr;
        int nCmdShow;
        String className;
    private:
        WNDCLASSEX wc{};
        DWORD currentStyle = 0;
        DWORD currentExStyle = 0;
    private:
        const DWORD windowedStyle = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_OVERLAPPED;
        const DWORD windowedStyleEx = 0;
        const DWORD fullscreenStyle = WS_POPUP;
        const DWORD fullscreenStyleEx = WS_EX_TOPMOST;
    private:
        void SetWindowPos(HWND after);
    };
}