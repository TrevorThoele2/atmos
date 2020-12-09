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

        [[nodiscard]] std::unique_ptr<Asset::Resource::Script> CreateAssetResource(
            const DataBuffer& buffer, String name) override;
        [[nodiscard]] std::unique_ptr<Atmos::Scripting::Resource> CreateScriptResource(
            const String& scriptAssetName, const String& executeName, const Parameters& parameters) override;
    public:
        File::Path Compile(
            const File::Path& inputFilePath, const std::optional<File::Path>& outputFilePath) override;
    protected:
        void InitializeImpl(Arca::Reliquary& reliquary) override;
    private:
        asIScriptEngine* engine = nullptr;
        UserData userData;

        asIScriptEngine& CreateEngine();

        void SetAndPushUserData(Manager& owner, Arca::Reliquary& reliquary);

        void RegisterAll();
    private:
        asIScriptModule& CreateModule(String name);
        [[nodiscard]] asIScriptModule& RequiredModule(String name);

        void BuildModule(asIScriptModule& module, String name, String fileData);
        void LoadModule(asIScriptModule& module, const DataBuffer& buffer);
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
        Logging::Logger* logger;
    };
}
