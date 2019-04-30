#pragma once

#include "InputManager.h"

namespace Atmos
{
    namespace Input
    {
        class NullManager : public Manager
        {
        private:
            void CreateSignalsImpl() override;
            void WorkSignalsImpl(SignalList& signalList) override;
            bool ShouldAddActions() const override;
            MousePosition GetMousePositionImpl() const override;
        };
    }
}