#pragma once

#include "WindowBase.h"

namespace Atmos
{
    class NullWindow : public WindowBase
    {
    public:
        NullWindow(ObjectManager& objectManager);

        void Setup() override;

        void Show() override;
        void Exit() override;
        bool IsCurrentlyFocused() const override;
        void Suspend(const TimeValue& time) override;
        bool OnStartFrame() override;
    protected:
        AxisBoundingBox2D AdjustWindowDimensions() override;
        void OnSetWindowDimensions() override;
        Position GetDefaultWindowPosition() override;
        void OnSetFullscreen() override;
    };
}