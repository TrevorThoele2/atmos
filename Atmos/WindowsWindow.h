#pragma once

#include "WindowBase.h"
#include "WindowsInclude.h"

#include "TimeValue.h"
#include "String.h"

namespace Atmos::Window
{
    class WindowsWindowBase : public WindowBase
    {
    public:
        virtual HWND GetHwnd() = 0;
    protected:
        WindowsWindowBase(ObjectManager& objectManager);
    };

    class WindowsWindow : public WindowsWindowBase
    {
    public:
        WindowsWindow(
            ObjectManager& objectManager,
            HINSTANCE hInstance,
            LPSTR lpCmdLine,
            int nCmdShow,
            const String& className);

        void Setup() override;

        void Show() override;
        void Exit() override;
        bool IsCurrentlyFocused() const override;
        void Suspend(const Time::Value& time);
        bool OnStartFrame() override;

        HWND GetHwnd() override;
    protected:
        AxisAlignedBox2D AdjustWindowDimensions() override;
        void OnSetWindowDimensions() override;
        Position GetDefaultWindowPosition() override;
        void OnSetFullscreen() override;
    private:
        HWND hwnd;
        int nCmdShow;
        String className;
    private:
        WNDCLASSEX wc;
        DWORD style;
        DWORD exStyle;
    private:
        const DWORD windowedStyle;
        const DWORD windowedStyleEx;
        const DWORD fullscreenStyle;
        const DWORD fullscreenStyleEx;
    private:
        void SetWindowPos(HWND after);
    };
}