#include "JavaScriptModule.h"

#include "JavaScriptString.h"
#include "JavaScriptVariant.h"
#include "JavaScriptVector.h"
#include "JavaScriptResult.h"
#include "JavaScriptUserData.h"
#include "JavaScriptMessage.h"
#include "ScriptError.h"

#include "StringUtility.h"
#include "FilePath.h"

namespace Atmos::Scripting::JavaScript
{
    std::weak_ptr<Module> Module::Create(
        v8::Isolate& isolate,
        const String& assetName,
        const String& sourceString)
    {
        return DoCreate(
            isolate,
            assetName,
            [&](const String& name, const v8::TryCatch& tryCatch)
            {
                const v8::ScriptOrigin origin(
                    ToV8(isolate, name),
                    0,
                    0,
                    false,
                    -1,
                    v8::Local<v8::Value>(),
                    false,
                    false,
                    true);
                v8::ScriptCompiler::Source source(ToV8(isolate, sourceString), origin);
                v8::Local<v8::Module> underlying;
                if (!v8::ScriptCompiler::CompileModule(&isolate, &source).ToLocal(&underlying))
                {
                    v8::String::Utf8Value error(&isolate, tryCatch.Exception());
                    throw Error(
                        "Could not compile JavaScript module.",
                        {
                            { "Reason", *error }
                        });
                }

                return underlying;
            });
    }

    std::weak_ptr<Module> Module::CreateSynthetic(
        v8::Isolate& isolate,
        const String& assetName,
        const std::vector<String>& exportNames,
        SyntheticDefinition definition)
    {
        return DoCreate(
            isolate,
            assetName,
            [&](const String& name, const v8::TryCatch&)
            {
                std::vector<v8::Local<v8::String>> useExportNames;
                useExportNames.reserve(exportNames.size());
                for (auto& exportName : exportNames)
                    useExportNames.emplace_back(ToV8(isolate, exportName));
                
                return v8::Module::CreateSyntheticModule(
                    &isolate,
                    ToV8(isolate, name),
                    useExportNames,
                    definition);
            });
    }

    void Module::Destroy(const Module& module)
    {
        modules.erase(module.name);
    }
    
	std::vector<Module*> Module::Modules()
    {
        std::vector<Module*> returnValue;
        returnValue.reserve(modules.size());
        for (auto& module : modules)
            returnValue.emplace_back(module.second.get());
        return returnValue;
    }
    
    Module::~Module()
    {
        const auto userData = static_cast<UserData*>(isolate->GetData(0));
        for (auto& listenFunctions : userData->listenFunctions)
        {
            for (auto contextualized = listenFunctions.second.begin(); contextualized != listenFunctions.second.end();)
            {
                if (contextualized->context == context)
                    contextualized = listenFunctions.second.erase(contextualized);
                else
                    ++contextualized;
            }
        }
    }

    void Module::Instantiate()
    {
        v8::Isolate::Scope isolateScope(isolate);

        const v8::TryCatch tryCatch(isolate);
        v8::HandleScope handleScope(isolate);

        const auto context = this->context.Get(isolate);
        v8::Context::Scope contextScope(context);
        const auto underlying = this->underlying.Get(isolate);

        if (underlying->InstantiateModule(context, &Resolve).IsNothing())
        {
            v8::String::Utf8Value error(isolate, tryCatch.Exception());
            throw Error(
                "Could not instantiate JavaScript script.",
                {
                    { "Reason", *error }
                });
        }

        v8::Local<v8::Value> result;
        if (!underlying->Evaluate(context).ToLocal(&result))
        {
            v8::String::Utf8Value error(isolate, tryCatch.Exception());
            throw Error(
                "Could not evaluate JavaScript script.",
                {
                    { "Reason", *error }
                });
        }
    }

    std::optional<Scripting::Result> Module::Execute(const Parameters& parameters, const String& executeFunctionName)
    {
        v8::Isolate::Scope isolateScope(isolate);

        v8::TryCatch tryCatch(isolate);
        v8::HandleScope handleScope(isolate);

        const auto context = this->context.Get(isolate);
        v8::Context::Scope contextScope(context);

        const auto underlying = this->underlying.Get(isolate);
        
        std::vector<v8::Local<v8::Value>> useParameters;
        for (auto& parameter : parameters)
            useParameters.push_back(ToV8(*isolate, parameter));

        const auto moduleNamespace = underlying->GetModuleNamespace()->ToObject(context).ToLocalChecked();
        const auto function = RetrieveFunction(context, moduleNamespace, executeFunctionName);
        const auto callResult = function->Call(
            context,
            moduleNamespace,
            static_cast<int>(parameters.size()),
            useParameters.data());
        const auto result = FromV8<Result>(*isolate, callResult);

        if (tryCatch.HasCaught())
        {
            const auto failure = ToFailure(*isolate, context, tryCatch.Message());
            const auto userData = static_cast<UserData*>(isolate->GetData(0));
            userData->reliquary->Do(ToLog(failure));
            return failure;
        }

        if (!result)
            return Variant{};

        if (!result->done)
            return {};

        return result->value ? result->value : Variant{};
    }

	String Module::Name() const
    {
        return name;
    }

    std::weak_ptr<Module> Module::DoCreate(
        v8::Isolate& isolate,
        const String& assetName,
        const Creator& creator)
    {
        v8::HandleScope handleScope(&isolate);

        const auto name = Name(assetName);
        
        v8::Isolate::Scope isolateScope(&isolate);
        const v8::TryCatch tryCatch(&isolate);

        const auto localContext = v8::Context::New(&isolate);
        const Context context(&isolate, localContext);
        v8::Context::Scope contextScope(localContext);

        const auto localUnderlying = creator(name, tryCatch);
        const Underlying underlying(&isolate, localUnderlying);

    	const auto created = modules.emplace(name, std::make_shared<Module>()).first->second;
        created->name = name;
        created->isolate = &isolate;
        created->underlying = underlying;
        created->context = context;
        return created;
    }

    v8::Local<v8::Function> Module::RetrieveFunction(
        const v8::Local<v8::Context>& context, const v8::Local<v8::Object>& object, const String& name)
    {
        const auto isolate = object->GetIsolate();
        const auto value = object->Get(
            context,
            ToV8(*isolate, name)).ToLocalChecked();
        
        if (!value->IsFunction())
        {
            const auto type = FromV8Required<String>(*isolate, value->TypeOf(isolate));
            const auto detailString = FromV8Required<String>(*isolate, value->ToDetailString(context));

            throw Error(
                "Retrieved value was not a function.",
                {
                    { "Name", name },
                    { "Type", type },
                    { "DetailString", detailString }
                });
        }
        return value.As<v8::Function>();
    }

    std::unordered_map<String, std::shared_ptr<Module>> Module::modules;

    v8::MaybeLocal<v8::Module> Module::Resolve(
        v8::Local<v8::Context> context, v8::Local<v8::String> specifier, v8::Local<v8::Module> referrer)
    {
        const auto isolate = context->GetIsolate();
        const auto moduleName = FromV8Required<String>(*isolate, specifier);
        const auto moduleFilePath = File::Path(moduleName);
        const auto moduleFileName = moduleFilePath.filename().string();
        const auto module = modules.find(moduleFileName);
        if (module == modules.end())
            throw Error(
                "Could not resolve module.",
                {
                    { "Name", moduleName }
                });
        return module->second->underlying.Get(isolate);
    }

    String Module::Name(const String& assetName)
    {
        const auto useAssetName = File::Path(assetName).replace_extension().string();

        auto currentName = useAssetName;
        int i = 0;
        auto found = modules.find(currentName);
        while (found != modules.end())
        {
            ++i;
            currentName = useAssetName + "_" + ToString(i);
            found = modules.find(currentName);
        }

        return currentName;
    }
}