#include "MockInputManager.h"

auto MockInputManager::ReadState() const -> State
{
    return sendState;
}