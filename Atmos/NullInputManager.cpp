#include "NullInputManager.h"

namespace Atmos::Input
{
    auto NullManager::ReadState() const -> State
    {
        return {};
    }
}