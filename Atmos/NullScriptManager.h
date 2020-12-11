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
        DataBuffer Compile(Module module, std::vector<Module> sharedModules) override;
    protected:
        void InitializeImpl(Arca::Reliquary& reliquary) override;
    };
}