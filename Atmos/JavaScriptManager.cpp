#include "JavaScriptManager.h"

#include <libplatform/libplatform.h>
#include "JavaScriptScriptResource.h"
#include "JavaScriptAssetResource.h"
#include "JavaScriptCompiler.h"
#include "JavaScriptRegisterAll.h"
#include "JavaScriptString.h"
#include "ScriptError.h"

#include <Inscription/Plaintext.h>

namespace Atmos::Scripting::JavaScript
{
	Manager::Manager(Logging::Logger& logger) :
		Scripting::Manager(logger, "JavaScript"),
		logger(&logger)
	{
		if (!initialized)
			Initialize(logger);

		bool hasAtmos = false;
		for (const auto& module : Module::Modules())
			if (module->Name() == atmosName)
				hasAtmos = true;

		if (!hasAtmos)
			CreateAtmosModule(*initialized->isolate);

		userData.isolate = initialized->isolate;
		initialized->isolate->SetData(0, &userData);
	}

	Manager::~Manager()
	{
		for (const auto& module : Module::Modules())
			Module::Destroy(*module);
	}

	void Manager::SetReliquary(Arca::Reliquary* reliquary)
	{
		userData.reliquary = reliquary;
	}

	[[nodiscard]] std::unique_ptr<Asset::Resource::Script> Manager::CreateAssetResource(
		const Buffer& buffer, String name)
	{
		v8::HandleScope handleScope(initialized->isolate);
		
		String source;
		for (auto& character : buffer)
			source.push_back(character);

		const auto module = Module::Create(*initialized->isolate, name, source);
		return std::make_unique<Resource::ScriptAsset>(source, module);
	}

	[[nodiscard]] std::unique_ptr<Scripting::Resource> Manager::CreateScriptResource(
		const Asset::Resource::Script& asset,
		const String& assetName,
		const String& executeName,
		const Parameters& parameters)
	{
		v8::HandleScope handleScope(initialized->isolate);
		
		auto& castedAsset = *dynamic_cast<const Resource::ScriptAsset*>(&asset);
		const auto module = Module::Create(*initialized->isolate, assetName, castedAsset.Source());
		return std::make_unique<ScriptResource>(module, parameters, executeName);
	}

	std::vector<CompiledModule> Manager::Compile(const std::vector<Scripting::Module>& modules)
	{
		return JavaScript::Compile(modules, *logger);
	}
	
	Manager::Initialized::Initialized(
		v8::Isolate& isolate,
		std::unique_ptr<v8::ArrayBuffer::Allocator>&& arrayAllocator,
		std::unique_ptr<v8::Platform>&& platform)
		:
		isolate(&isolate),
		arrayAllocator(std::move(arrayAllocator)),
		platform(std::move(platform))
	{}

	Manager::Initialized::Initialized(Initialized&& arg) noexcept :
		isolate(arg.isolate), arrayAllocator(std::move(arg.arrayAllocator)), platform(std::move(arg.platform))
	{
		arg.isolate = nullptr;
	}

	Manager::Initialized& Manager::Initialized::operator=(Initialized&& arg) noexcept
	{
		isolate = arg.isolate;
		arg.isolate = nullptr;
		arrayAllocator = std::move(arg.arrayAllocator);
		platform = std::move(arg.platform);
		return *this;
	}

	Manager::Initialized::~Initialized()
	{
		if (isolate)
		{
			isolate->Dispose();
			v8::V8::Dispose();
			v8::V8::DisposePlatform();
		}
	}

	std::optional<Manager::Initialized> Manager::initialized;

	const String Manager::atmosName = "atmos";

	void Manager::Initialize(Logging::Logger& logger)
	{
		logger.Log("Initializing V8.");

		const auto errorFileName = String("v8errors.txt");
		const auto errorFile = freopen(errorFileName.c_str(), "w", stderr);

		if (!v8::V8::InitializeICU("icudtl.dat"))
			throw Error("Initializing V8 ICU failed.");
		logger.Log("Initialized V8 ICU.");

		auto platform = v8::platform::NewDefaultPlatform();
		if (!platform)
			throw Error("Initializing V8 platform failed.");
		logger.Log("Initialized V8 platform.");

		v8::V8::InitializePlatform(platform.get());
		if (!v8::V8::Initialize())
			throw Error("Initializing V8 failed");
		logger.Log("Initialized V8.");

		auto arrayAllocator = std::unique_ptr<v8::ArrayBuffer::Allocator>(v8::ArrayBuffer::Allocator::NewDefaultAllocator());

		v8::Isolate::CreateParams createParameters;
		createParameters.array_buffer_allocator = arrayAllocator.get();
		const auto isolate = v8::Isolate::New(createParameters);

		isolate->AddMessageListener(&ErrorCallback);

		initialized = Initialized{ *isolate, std::move(arrayAllocator), std::move(platform) };

		if (errorFile)
		{
			fclose(errorFile);
			const auto errorContents = Inscription::Plaintext::FromFile(errorFileName);
			if (errorContents.empty())
				std::remove(errorFileName.c_str());
		}
	}

	std::weak_ptr<Module> Manager::CreateAtmosModule(v8::Isolate& isolate)
	{
		const auto exportNames = std::vector<String>{ "Atmos" };
		const auto definition = &RegisterAll;
		return Module::CreateSynthetic(isolate, atmosName, exportNames, definition);
	}

	void Manager::ErrorCallback(v8::Local<v8::Message> message, v8::Local<v8::Value> data)
	{
		const auto isolate = message->GetIsolate();
		const auto context = isolate->GetCurrentContext();

		const auto errorMessage = FromV8Required<String>(*isolate, message->Get());

		const auto userData = static_cast<UserData*>(isolate->GetData(0));
		userData->reliquary->Do(Logging::Log{
			errorMessage,
			Logging::Severity::Error,
			Logging::Details
			{
				{ "Line", ToString(message->GetLineNumber(context).ToChecked()) }
			} });
	}
}