#pragma once

#include "ScriptManager.h"
#include <v8.h>

#include "JavaScriptModule.h"
#include "JavaScriptUserData.h"

namespace Atmos::Scripting::JavaScript
{
	class Manager final : public Scripting::Manager
	{
	public:
		Manager(Logging::Logger& logger);
		~Manager();

		void SetReliquary(Arca::Reliquary* reliquary) override;

		[[nodiscard]] std::unique_ptr<Asset::Resource::Script> CreateAssetResource(
			const Buffer& buffer, String name) override;
		[[nodiscard]] std::unique_ptr<Scripting::Resource> CreateScriptResource(
			const Asset::Resource::Script& asset,
			const String& assetName,
			const String& executeName,
			const Parameters& parameters) override;

		std::vector<CompiledModule> Compile(const std::vector<Scripting::Module>& modules) override;
	private:
		UserData userData;

		struct Initialized
		{
			v8::Isolate* isolate = nullptr;
			std::unique_ptr<v8::ArrayBuffer::Allocator> arrayAllocator;
			std::unique_ptr<v8::Platform> platform;

			Initialized() = default;
			Initialized(
				v8::Isolate& isolate,
				std::unique_ptr<v8::ArrayBuffer::Allocator>&& arrayAllocator,
				std::unique_ptr<v8::Platform>&& platform);
			Initialized(Initialized&& arg) noexcept;
			Initialized& operator=(Initialized&& arg) noexcept;
			~Initialized();
		};

		static std::optional<Initialized> initialized;

		static void Initialize(Logging::Logger& logger);
	private:
		static const String atmosName;

		static std::weak_ptr<Module> CreateAtmosModule(v8::Isolate& isolate);
	private:
		Logging::Logger* logger;

		static void ErrorCallback(v8::Local<v8::Message> message, v8::Local<v8::Value> data);
	};
}