#include "MockInputManager.h"

MockInputManager::MockInputManager(Atmos::Logging::Logger& logger) : Manager(logger, "Mock")
{}

auto MockInputManager::ReadState() const -> State
{
    return sendState;
}