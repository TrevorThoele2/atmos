#include "AngelScriptAssetData.h"

namespace Atmos::Script::Angel
{
    AssetDataImplementation::AssetDataImplementation(asIScriptModule& module) : module(&module)
    {}

    asIScriptModule* AssetDataImplementation::Module() const
    {
        return module;
    }
}