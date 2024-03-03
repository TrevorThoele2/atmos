#include "AngelScriptManager.h"

#include <angelscript.h>
#include "AngelScriptAssetResource.h"
#include "AngelScriptScriptResource.h"
#include "AngelScriptResultVerification.h"
#include "AngelScriptRegistration.h"

#include <Arca/Reliquary.h>

#include "Log.h"

#include "StringUtility.h"

#include <Inscription/InputTextFile.h>
#include "OutputAngelScriptBytecodeStream.h"
#include "InputAngelScriptBytecodeStream.h"
#include "AngelScriptCompiledFileExtension.h"

namespace Atmos::Scripting::Angel
{
    Manager::Manager(Logging::Logger& logger) : engine(&CreateEngine()), logger(&logger)
    {}

    Manager::~Manager()
    {
        VerifyResult(engine->ShutDownAndRelease());
    }

    std::unique_ptr<Asset::Resource::Script> Manager::CreateAssetResource(
        const Buffer& buffer, String name)
    {
        const auto moduleName = ScriptAssetNameToModuleName(name);
        auto& module = CreateModule(moduleName);
        LoadModule(module, buffer);
        return std::make_unique<Resource::Script>(module);
    }

    std::unique_ptr<Atmos::Scripting::Resource> Manager::CreateScriptResource(
        const String& name, const String& executeName, const Parameters& parameters)
    {
        const auto moduleName = ScriptAssetNameToModuleName(name);
        auto& module = RequiredModule(moduleName);
        auto& function = RequiredFunction(module, executeName);

        auto& context = CreateContext();

        VerifyResult(context.SetExceptionCallback(asMETHOD(Manager, ContextMessageCallback), this, asCALL_THISCALL));

        return std::make_unique<ScriptResource>(parameters, function, context, *logger);
    }

    Buffer Manager::Compile(Module module, std::vector<Module> sharedModules)
    {
        Buffer returnValue;

        try
        {
            auto& createdModule = CreateModule(module.name);
            AddScriptSection(createdModule, module.name, module.data);
            for (auto& sharedModule : sharedModules)
                AddScriptSection(createdModule, sharedModule.name, sharedModule.data);
            BuildModule(createdModule);

            auto outputStream = OutputBytecodeStream();
            VerifyResult(createdModule.SaveByteCode(&outputStream));
            returnValue = outputStream.Buffer();
        }
        catch(...)
        {
            logger->Log(
                "Compilation of script failed.",
                Logging::Severity::Information,
                { { { "Name", module.name } } });
            throw;
        }

        logger->Log(
            "Compilation of script succeeded.",
            Logging::Severity::Information,
            { { { "Name", module.name } } });

        return returnValue;
    }

    void Manager::InitializeImpl(Arca::Reliquary& reliquary)
    {
        SetAndPushUserData(*this, reliquary);
        RegisterAll();
    }

    asIScriptEngine& Manager::CreateEngine()
    {
        auto engine = asCreateScriptEngine();
        if (!engine)
            throw AngelScriptFailed("Creation of the scripting engine has failed.");

        VerifyResult(engine->SetMessageCallback(asMETHOD(Manager, EngineMessageCallback), this, asCALL_THISCALL));

        return *engine;
    }

    void Manager::SetAndPushUserData(Manager& owner, Arca::Reliquary& reliquary)
    {
        userData.reliquary = &reliquary;
        userData.manager = &owner;

        engine->SetUserData(&userData);
    }

    void Manager::RegisterAll()
    {
        Angel::RegisterAll(*engine);
    }

    asIScriptModule& Manager::CreateModule(String name)
    {
        const auto module = engine->GetModule(name.c_str(), asGM_ALWAYS_CREATE);
        if (!module)
            throw AngelScriptFailed(
                "Module could not be created.",
                {
                    {"Module Name", name}
                });

        return *module;
    }

    asIScriptModule& Manager::RequiredModule(String name)
    {
        const auto module = engine->GetModule(name.c_str(), asGM_ONLY_IF_EXISTS);
        if (!module)
            throw AngelScriptFailed(
                "Module could not be found.",
                {
                    {"Module Name", name}
                });

        return *module;
    }

    void Manager::AddScriptSection(asIScriptModule& module, String name, String fileData)
    {
        VerifyResult(module.AddScriptSection(
            name.c_str(),
            fileData.c_str(),
            fileData.length()));
    }

    void Manager::BuildModule(asIScriptModule& module)
    {
        VerifyResult(module.Build());
    }

    void Manager::LoadModule(asIScriptModule& module, const Buffer& buffer)
    {
        auto inputStream = InputBytecodeStream(buffer);

        VerifyResult(module.LoadByteCode(&inputStream));
    }

    asIScriptContext& Manager::CreateContext()
    {
        const auto context = engine->RequestContext();
        if (!context)
            throw AngelScriptFailed("Context could not be created.");

        return *context;
    }

    asIScriptFunction& Manager::RequiredFunction(asIScriptModule& module, String name)
    {
        const auto function = module.GetFunctionByName(name.c_str());
        if (!function)
            throw AngelScriptFailed(
                "Function could not be found.",
                {
                    {"Module Name", module.GetName()},
                    {"Execute Name", name}
                });

        return *function;
    }

    void Manager::EngineMessageCallback(const asSMessageInfo* messageInfo)
    {
        if (!messageInfo->message)
        {
            logger->Log(Logging::Log{
                "AngelScript execution failed.",
                Logging::Severity::Error });
            return;
        }

        auto logType = Logging::Severity::Error;
        if (messageInfo->type == asMSGTYPE_WARNING)
            logType = Logging::Severity::Warning;
        else if (messageInfo->type == asMSGTYPE_INFORMATION)
            logType = Logging::Severity::Information;

        logger->Log(Logging::Log{
            messageInfo->message,
            logType,
            Logging::Details
            {
                NameValuePair("Section", String(messageInfo->section)),
                NameValuePair("Row", ToString(messageInfo->row)),
                NameValuePair("Column", ToString(messageInfo->col)),
            } });
    }

    void Manager::ContextMessageCallback(asIScriptContext* context)
    {
        int column = 0;
        const char* section = nullptr;
        const auto line = context->GetExceptionLineNumber(&column, &section);
        const String sectionName = section
            ? section
            : "";

        const String message = context->GetExceptionString();

        logger->Log(Logging::Log{
            message,
            Logging::Severity::Error,
            Logging::Details
            {
                NameValuePair("Section", sectionName),
                NameValuePair("Row", Chroma::ToString(line)),
                NameValuePair("Column", Chroma::ToString(column)),
            } });
    }

    String Manager::ScriptAssetNameToModuleName(const String& scriptAssetName)
    {
        const auto foundDot = scriptAssetName.find('.');
        return scriptAssetName.substr(0, foundDot);
    }
}