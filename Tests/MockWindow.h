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
    void OnPositionChanged() override;
    void OnSizeChanged() override;
    void OnFullscreenChanged() override;
    [[nodiscard]] Size WindowSizeFromClientSize() const override;

    [[nodiscard]] Size TotalScreenSize() const override;
private:
    bool hasRan = false;
};