#include "AngelScriptManager.h"
#include "AngelScriptAssetData.h"
#include "AngelScriptResultVerification.h"

namespace Atmos::Script::Angel
{
    std::unique_ptr<Asset::ScriptAssetData> Manager::CreateAssetData(String name, String fileData)
    {
        auto module = engine->GetModule(name.c_str(), asGM_ALWAYS_CREATE);

        VerifyResult(module->AddScriptSection
        (
            name.c_str(),
            fileData.c_str(),
            fileData.length()
        ));

        module->Build();

        return std::make_unique<AssetDataImplementation>(*module);
    }
}
