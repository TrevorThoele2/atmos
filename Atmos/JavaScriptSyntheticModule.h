#pragma once

#include <v8.h>
#include "String.h"

namespace Atmos::Scripting::JavaScript
{
	void SetSyntheticModuleExport(
		v8::Isolate& isolate, const v8::Local<v8::Module>& module, const String& name, const v8::Local<v8::Value>& value);
}