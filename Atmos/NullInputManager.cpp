#include "NullInputManager.h"

namespace Atmos::Input
{
    std::vector<NullManager::CreatedInput> NullManager::CreateAllInputs() const
    {
        return {};
    }

    ScreenPosition NullManager::CurrentMousePosition() const
    {
        return {};
    }

    bool NullManager::IsInputDown(Input& input) const
    {
        return false;
    }
}