#include "NullInputManager.h"

namespace Atmos::Input
{
    auto NullManager::ReadState() const -> State
    {
        return {};
    }

    String NullManager::TypeName() const
    {
        return "Null";
    }
}