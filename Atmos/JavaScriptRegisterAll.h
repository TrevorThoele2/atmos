#pragma once

#include <v8.h>
#include "String.h"

namespace Atmos::Scripting::JavaScript
{
	v8::MaybeLocal<v8::Value> RegisterAll(v8::Local<v8::Context> context, v8::Local<v8::Module> module);
}