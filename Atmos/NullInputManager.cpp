#include "NullInputManager.h"

namespace Atmos::Input
{
    NullManager::NullManager(Logging::Logger& logger) : Manager(logger, "Null")
    {}

    auto NullManager::ReadState() const -> State
    {
        return {};
    }
}