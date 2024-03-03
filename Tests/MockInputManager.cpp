#include "MockInputManager.h"

auto MockInputManager::ReadState() const -> State
{
    return sendState;
}

Atmos::String MockInputManager::TypeName() const
{
    return "Mock";
}