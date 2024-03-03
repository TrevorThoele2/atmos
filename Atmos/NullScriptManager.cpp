#include "NullScriptManager.h"

namespace Atmos::Scripting
{
    std::unique_ptr<Asset::Resource::Script> NullManager::CreateAssetResource(
        const DataBuffer& buffer, String name)
    {
        return {};
    }

    std::unique_ptr<Resource> NullManager::CreateScriptResource(
        const String& scriptAssetName, const String& executeName, const Parameters& parameters)
    {
        return {};
    }

    File::Path NullManager::Compile(
        const File::Path& inputFilePath, const std::optional<File::Path>& outputFilePath)
    {
        return {};
    }

    void NullManager::InitializeImpl(Arca::Reliquary& reliquary)
    {

    }
}