#include "AngelScriptAssetData.h"

namespace Atmos::Script::Angel
{
    AssetDataImplementation::AssetDataImplementation(asIScriptModule& module) : module(&module)
    {}

    std::unique_ptr<Asset::ScriptAssetData> AssetDataImplementation::Clone() const
    {
        return std::make_unique<AssetDataImplementation>(*this);
    }

    asIScriptModule* AssetDataImplementation::Module() const
    {
        return module;
    }
}