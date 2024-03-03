#pragma once

#include <v8.h>

namespace Atmos::Scripting::JavaScript
{
	[[nodiscard]] v8::Local<v8::Function> CreateFunction(
		v8::Isolate& isolate,
		const v8::Local<v8::Context>& context,
		v8::FunctionCallback functionCallback);
}