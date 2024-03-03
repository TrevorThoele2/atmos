#pragma once

#include "WindowBase.h"

namespace Atmos::Window
{
    class NullWindow : public WindowBase
    {
    public:
        NullWindow(ObjectManager& objectManager);

        void Setup() override;

        void Show() override;
        void Exit() override;
        bool IsCurrentlyFocused() const override;
        void Suspend(const Time::Value& time) override;
        bool OnStartFrame() override;
    protected:
        AxisAlignedBox2D AdjustWindowDimensions() override;
        void OnSetWindowDimensions() override;
        Position GetDefaultWindowPosition() override;
        void OnSetFullscreen() override;
    };
}