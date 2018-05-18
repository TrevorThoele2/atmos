
#pragma once

#include "WindowsInclude.h"

#include "Environment.h"

namespace Atmos
{
    class WindowsBase : public EnvironmentModel
    {
    public:
        virtual HWND GetHwnd() = 0;
    };

    class Windows : public WindowsBase
    {
    private:
        int nCmdShow;
        HWND hWnd;
        DWORD style;
        DWORD exStyle;
        WNDCLASSEX wc;
        String className;

        const DWORD windowedStyle;
        const DWORD windowedStyleEx;
        const DWORD fullscreenStyle;
        const DWORD fullscreenStyleEx;

        void SetupImpl() override;
        AxisBoundingBox2D AdjustWindowDimensions() override;
        void OnSetWindowDimensions() override;
        Position2D GetDefaultWindowPosition() override;
        void OnSetFullscreen() override;
        void SetWindowPos(HWND after);
    public:
        Windows(HINSTANCE hInstance, LPSTR lpCmdLine, int nCmdShow, const String &className);
        void Show() override;
        bool StartFrame() override;
        void Exit() override;
        bool IsFocused() const override;

        void CreateInput() override;
        void CreateGraphics() override;
        void CreateAudio() override;
        void CreateFileSystem() override;

        HWND GetHwnd() override;
    };
}