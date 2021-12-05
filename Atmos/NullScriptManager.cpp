#include "NullScriptManager.h"

namespace Atmos::Scripting
{
    NullManager::NullManager(Logging::Logger& logger) : Manager(logger, "Null")
    {}

    void NullManager::SetReliquary(Arca::Reliquary* reliquary)
    {}

    std::unique_ptr<Asset::Resource::Script> NullManager::CreateAssetResource(
        const Buffer& buffer, String name)
    {
        return {};
    }

    std::unique_ptr<Resource> NullManager::CreateScriptResource(
        const Asset::Resource::Script& asset,
        const String& assetName,
        const String& executeName,
        const Parameters& parameters)
    {
        return {};
    }

    std::vector<CompiledModule> NullManager::Compile(const std::vector<Module>& modules)
    {
        return {};
    }
}