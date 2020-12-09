#pragma once

#include <memory>
#include "ScriptAssetResource.h"
#include "ScriptResource.h"
#include "ScriptParameters.h"

#include "String.h"
#include "FilePath.h"
#include "Buffer.h"

#include <Arca/Reliquary.h>

namespace Atmos::Scripting
{
    class Manager
    {
    public:
        virtual ~Manager() = 0;

        void Initialize(Arca::Reliquary& reliquary);

        [[nodiscard]] virtual std::unique_ptr<Asset::Resource::Script> CreateAssetResource(
            const DataBuffer& buffer, String name) = 0;

        [[nodiscard]] virtual std::unique_ptr<Resource> CreateScriptResource(
            const String& scriptAssetName, const String& executeName, const Parameters& parameters) = 0;
    public:
        virtual File::Path Compile(
            const File::Path& inputFilePath, const std::optional<File::Path>& outputFilePath) = 0;
    protected:
        virtual void InitializeImpl(Arca::Reliquary& reliquary) = 0;
    private:
        bool isInitialized = false;
    };
}