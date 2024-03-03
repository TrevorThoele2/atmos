#include "NullInputManager.h"

namespace Atmos::Input
{
    std::vector<NullManager::CreatedInput> NullManager::CreateAllInputs() const
    {
        return {};
    }

    Spatial::ScreenPoint NullManager::CurrentMousePosition() const
    {
        return {};
    }

    bool NullManager::IsInputDown(Input& input) const
    {
        return false;
    }
}