#pragma once

#include "InputManager.h"

namespace Atmos::Window
{
    class WindowSystem;
}

namespace Atmos::Input
{
    class WindowsManager : public Manager
    {
    public:
        WindowsManager() = default;
    private:
        void CreateSignalsImpl() override;
        void WorkSignalsImpl(SignalList& signalList) override;
        bool ShouldAddActions() const override;
        MousePosition GetMousePositionImpl() const override;
    private:
        bool IsSignalDownBase(SignalBase& signal) const;
        bool IsMouseWithinScreen() const;
    private:
        Window::WindowSystem* FindWindowSystem() const;
    };
}