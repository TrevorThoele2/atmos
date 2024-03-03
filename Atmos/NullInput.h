#pragma once

#include "InputManager.h"

namespace Atmos::Input
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