#pragma once

#include "JavaScriptType.h"
#include <optional>

namespace Atmos::Scripting::JavaScript
{
	template<class T>
	class Type<std::optional<T>>
	{
	public:
		using V8T = v8::Value;

		[[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const std::optional<T>& value);
		[[nodiscard]] static std::optional<T> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
	};

	template<class T>
	auto Type<std::optional<T>>::ToV8(v8::Isolate& isolate, const std::optional<T>& value) -> v8::Local<V8T>
	{
		return !value ? v8::Null(&isolate).As<v8::Value>() : JavaScript::ToV8(isolate, *value).template As<v8::Value>();
	}

	template<class T>
	std::optional<T> Type<std::optional<T>>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
	{
		return value->IsNullOrUndefined() ? std::optional<T>{} : JavaScript::FromV8<T>(isolate, value);
	}
}