#include "NullScriptManager.h"

namespace Atmos::Scripting
{
    NullManager::NullManager(Logging::Logger& logger) : Manager(logger, "Null")
    {}

    void NullManager::SetReliquary(Arca::Reliquary& reliquary)
    {}

    std::unique_ptr<Asset::Resource::Script> NullManager::CreateAssetResource(
        const Buffer& buffer, String name)
    {
        return {};
    }

    std::unique_ptr<Resource> NullManager::CreateScriptResource(
        const String& scriptAssetName, const String& executeName, const Parameters& parameters)
    {
        return {};
    }

    Buffer NullManager::Compile(Module module, std::vector<Module> sharedModules)
    {
        return {};
    }
}