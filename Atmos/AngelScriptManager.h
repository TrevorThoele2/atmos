#pragma once

#include "ScriptManager.h"
#include "AngelScriptUserData.h"
#include <memory>

#include "Logger.h"

class asIScriptEngine;
class asIScriptModule;
class asIScriptContext;
class asIScriptFunction;
struct asSMessageInfo;

namespace Atmos::Scripting::Angel
{
    class Manager final : public Atmos::Scripting::Manager
    {
    public:
        Manager(Logging::Logger& logger);
        ~Manager();

        void SetReliquary(Arca::Reliquary* reliquary) override;

        [[nodiscard]] std::unique_ptr<Asset::Resource::Script> CreateAssetResource(
            const Buffer& buffer, String name) override;
        [[nodiscard]] std::unique_ptr<Atmos::Scripting::Resource> CreateScriptResource(
            const String& name, const String& executeName, const Parameters& parameters) override;

        Buffer Compile(Module module, std::vector<Module> sharedModules) override;
    private:
        asIScriptEngine* engine = nullptr;
        UserData userData;

        asIScriptEngine& CreateEngine();
    private:
        std::vector<asIScriptModule*> sharedModules;

        asIScriptModule& CreateModule(String name);
        [[nodiscard]] asIScriptModule& RequiredModule(String name);

        void AddScriptSection(asIScriptModule& module, String name, String fileData);
        void BuildModule(asIScriptModule& module);
        void LoadModule(asIScriptModule& module, const Buffer& buffer);
    private:
        asIScriptContext& CreateContext();
    private:
        [[nodiscard]] asIScriptFunction& RequiredFunction(asIScriptModule& module, String name);
    private:
        void EngineMessageCallback(const asSMessageInfo* messageInfo);
        void ContextMessageCallback(asIScriptContext* context);
    private:
        [[nodiscard]] static String ScriptAssetNameToModuleName(const String& scriptAssetName);
    private:
        Arca::Reliquary* reliquary = nullptr;

        void AttemptDestroyAllScripts();
    private:
        Logging::Logger* logger;
    };
}
