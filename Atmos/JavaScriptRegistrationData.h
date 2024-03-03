#pragma once

#include <v8.h>

namespace Atmos::Scripting::JavaScript
{
	struct RegistrationData
	{
		v8::Isolate* isolate;
		v8::Local<v8::Context> context;
		v8::Local<v8::Module> module;
	};
}