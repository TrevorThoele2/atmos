#include "InputManager.h"

namespace Atmos::Input
{
    Manager::~Manager() = default;
}

namespace Arca
{
    const TypeName Traits<Atmos::Input::Manager>::typeName = "InputManager";
}