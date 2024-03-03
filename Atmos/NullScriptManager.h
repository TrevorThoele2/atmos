#pragma once

#include "ScriptManager.h"

namespace Atmos::Scripting
{
    class NullManager final : public Manager
    {
    public:
        [[nodiscard]] std::unique_ptr<Asset::Resource::Script> CreateAssetResource(
            const DataBuffer& buffer, String name) override;
        [[nodiscard]] std::unique_ptr<Resource> CreateScriptResource(
            const String& scriptAssetName, const String& executeName, const Parameters& parameters) override;
    public:
        File::Path Compile(
            const File::Path& inputFilePath, const std::optional<File::Path>& outputFilePath) override;
    protected:
        void InitializeImpl(Arca::Reliquary& reliquary) override;
    };
}