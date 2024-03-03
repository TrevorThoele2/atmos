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
        WindowsWindow(Arca::Reliquary& reliquary, int nCmdShow, const String& className);

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
        DWORD style = 0;
        DWORD exStyle = 0;
    private:
        const DWORD windowedStyle;
        const DWORD windowedStyleEx;
        const DWORD fullscreenStyle;
        const DWORD fullscreenStyleEx;
    private:
        void SetWindowPos(HWND after);
    };
}