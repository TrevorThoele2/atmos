#pragma once

#include "WindowBase.h"

namespace Atmos::Window
{
    class NullWindow final : public WindowBase
    {
    public:
        void Show() override;
        void Exit() override;
        [[nodiscard]] bool IsCurrentlyFocused() const override;
        void Suspend(const Time::Value& time) override;
        bool OnStartFrame() override;
    protected:
        void SetupImpl() override;

        AxisAlignedBox2D AdjustWindowDimensions() override;
        void OnSetWindowDimensions() override;
        Position GetDefaultWindowPosition() override;
        void OnSetFullscreen() override;
    };
}