#pragma once

#include <Atmos/WindowBase.h>

using namespace Atmos;

class MockWindow final : public Window::WindowBase
{
public:
    void Show() override;
    void Exit() override;
    [[nodiscard]] bool IsCurrentlyFocused() const override;
    void Suspend(const Time::Duration<>& time) override;
    bool OnStartFrame() override;

    [[nodiscard]] void* Handle() const override;
protected:
    void SetupImpl() override;

    Spatial::AxisAlignedBox2D AdjustWindowDimensions() override;
    void OnSetWindowDimensions() override;
    Position GetDefaultWindowPosition() override;
    void OnSetFullscreen() override;
private:
    bool hasRan = false;
};