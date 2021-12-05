#pragma once

#include <unordered_map>
#include <v8.h>

#include "JavaScriptPersistent.h"
#include "ScriptResult.h"
#include "ScriptParameters.h"

#include "String.h"

namespace Atmos::Scripting::JavaScript
{
	class Module
	{
	public:
        using SyntheticDefinition = v8::MaybeLocal<v8::Value>(*)(v8::Local<v8::Context>, v8::Local<v8::Module>);
    public:
        static std::weak_ptr<Module> Create(
            v8::Isolate& isolate,
            const String& assetName,
            const String& sourceString);
        static std::weak_ptr<Module> CreateSynthetic(
            v8::Isolate& isolate,
            const String& assetName,
            const std::vector<String>& exportNames,
            SyntheticDefinition definition);
        static void Destroy(const Module& module);
        static std::vector<Module*> Modules();
    public:
        Module() = default;
        ~Module();

        void Instantiate();

        std::optional<Scripting::Result> Execute(const Parameters& parameters, const String& executeFunctionName);

        [[nodiscard]] String Name() const;
	private:
        using Creator = std::function<v8::Local<v8::Module>(const String&, const v8::TryCatch&)>;

        static std::weak_ptr<Module> DoCreate(
			v8::Isolate& isolate,
            const String& assetName,
            const Creator& creator);
    private:
        String name;

        v8::Isolate* isolate;

        using Underlying = CopyablePersistent<v8::Module>;
        using Context = CopyablePersistent<v8::Context>;

        Underlying underlying;
        Context context;

        [[nodiscard]] static v8::Local<v8::Function> RetrieveFunction(
            const v8::Local<v8::Context>& context, const v8::Local<v8::Object>& object, const String& name);
	private:
        static std::unordered_map<String, std::shared_ptr<Module>> modules;

        [[nodiscard]] static v8::MaybeLocal<v8::Module> Resolve(
            v8::Local<v8::Context> context, v8::Local<v8::String> specifier, v8::Local<v8::Module> referrer);

        [[nodiscard]] static String Name(const String& assetName);
	};
}