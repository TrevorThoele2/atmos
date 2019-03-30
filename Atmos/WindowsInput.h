#pragma once

#include "InputHandlerBase.h"

namespace Atmos
{
    namespace Input
    {
        class WindowsHandler : public HandlerBase
        {
        private:
            void AddInputsImpl() override;
            void WorkInputsImpl(InputVector &inputs) override;
            bool ShouldAddActions() const override;
            MousePosition GetMousePositionImpl() const override;

            bool IsKeyDownBase(SignalBase &signal) const;
            bool IsMouseWithinScreen() const;
        public:
            WindowsHandler() = default;
        };
    }
}