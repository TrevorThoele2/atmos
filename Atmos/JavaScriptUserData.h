#pragma once

#include <Arca/Reliquary.h>
#include <unordered_map>
#include <functional>
#include "JavaScriptPersistent.h"

namespace Atmos::Scripting::JavaScript
{
	class UserData
	{
	public:
		v8::Isolate* isolate;

		Arca::Reliquary* reliquary = nullptr;

		std::unordered_map<String, std::function<void(v8::Isolate&, const v8::Local<v8::Value>&, v8::ReturnValue<v8::Value>, Arca::Reliquary&)>> sendFunctions;

		struct ContextualizedFunction
		{
			CopyablePersistent<v8::Context> context;
			std::vector<CopyablePersistent<v8::Function>> functions;
		};
		std::unordered_map<String, std::vector<ContextualizedFunction>> listenFunctions;

		std::unordered_map<String, std::function<v8::Local<v8::Value>(v8::Isolate&, const v8::FunctionCallbackInfo<v8::Value>&, Arca::Reliquary&)>> findFunctions;
	};
}