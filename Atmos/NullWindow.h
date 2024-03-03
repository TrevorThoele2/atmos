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
        void Suspend(const Time::Duration<>& time) override;
        bool OnStartFrame() override;

        [[nodiscard]] void* Handle() const override;

        [[nodiscard]] String TypeName() const override;
    protected:
        void OnPositionChanged() override;
        void OnSizeChanged() override;
        void OnFullscreenChanged() override;
        [[nodiscard]] Size WindowSizeFromClientSize() const override;

        [[nodiscard]] Size TotalScreenSize() const override;
    };
}