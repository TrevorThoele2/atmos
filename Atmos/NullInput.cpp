
#include "NullInput.h"

namespace Atmos::Input
{
    void NullManager::CreateSignalsImpl()
    {}

    void NullManager::WorkSignalsImpl(SignalList& signalList)
    {}

    bool NullManager::ShouldAddActions() const
    {
        return false;
    }

    MousePosition NullManager::GetMousePositionImpl() const
    {
        return MousePosition();
    }
}