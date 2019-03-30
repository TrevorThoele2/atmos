#pragma once

#include "InputHandlerBase.h"

namespace Atmos
{
    namespace Input
    {
        class NullHandler : public HandlerBase
        {
        private:
            void AddInputsImpl() override;
            void WorkInputsImpl(InputVector &inputs) override;
            bool ShouldAddActions() const override;
            MousePosition GetMousePositionImpl() const override;
        };
    }
}