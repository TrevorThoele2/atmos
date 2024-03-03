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
        WindowsWindow(HINSTANCE hInstance, int nCmdShow, const String& className);

        void Show() override;
        void Exit() override;
        [[nodiscard]] bool IsCurrentlyFocused() const override;
        void Suspend(const Time::Duration<>& time) override;
        bool OnStartFrame() override;

        [[nodiscard]] void* Handle() const override;
    protected:
        void SetupImpl() override;

        void OnPositionChanged() override;
        void OnSizeChanged() override;
        void OnFullscreenChanged() override;
        [[nodiscard]] Size WindowSizeFromClientSize() const override;

        [[nodiscard]] Size TotalScreenSize() const override;
    private:
        HWND hwnd = nullptr;
        int nCmdShow;
        String className;
    private:
        WNDCLASSEX windowClass{};
        DWORD currentStyle = 0;
        DWORD currentExStyle = 0;
    private:
        const DWORD windowedStyle = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_OVERLAPPED;
        const DWORD windowedStyleEx = 0;
        const DWORD fullscreenStyle = WS_POPUP;
        const DWORD fullscreenStyleEx = WS_EX_TOPMOST;
    private:
        void ChangeStyles();
        void SetWindowPositionAndSize(HWND after);
    private:
        static String LastErrorMessage();
    };
}