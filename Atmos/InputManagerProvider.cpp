#include "InputManagerProvider.h"

#include "NullInputManager.h"

namespace Atmos::Input
{
    ManagerProvider::ManagerProvider()
    {
        value = std::make_unique<NullManager>();
    }
}

namespace Arca
{
    const TypeName Traits<Atmos::Input::ManagerProvider>::typeName = "InputManagerProvider";
}