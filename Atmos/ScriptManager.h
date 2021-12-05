#pragma once

#include <memory>
#include "ScriptAssetResource.h"
#include "ScriptResource.h"
#include "ScriptParameters.h"
#include "ScriptModule.h"
#include "ScriptCompiledModule.h"

#include "String.h"
#include "Buffer.h"

#include "Logger.h"

namespace Atmos::Scripting
{
    class Manager
    {
    public:
        virtual ~Manager() = 0;

        virtual void SetReliquary(Arca::Reliquary* reliquary) = 0;

        [[nodiscard]] virtual std::unique_ptr<Asset::Resource::Script> CreateAssetResource(
            const Buffer& buffer, String name) = 0;
        [[nodiscard]] virtual std::unique_ptr<Resource> CreateScriptResource(
            const Asset::Resource::Script& asset,
            const String& name,
            const String& executeName,
            const Parameters& parameters) = 0;

        virtual std::vector<CompiledModule> Compile(const std::vector<Module>& modules) = 0;
    protected:
        Manager(Logging::Logger& logger, String typeName);
    private:
        String typeName;
        Logging::Logger* logger;
    };
}