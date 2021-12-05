#pragma once

#include <v8.h>
#include <optional>
#include "ScriptError.h"
#include "JavaScriptRegistrationData.h"

namespace Atmos::Scripting::JavaScript
{
	template<class T>
	class Type;

	template<class T>
	[[nodiscard]] v8::Local<typename Type<T>::V8T> ToV8(v8::Isolate& isolate, const T& value)
	{
		return Type<T>::ToV8(isolate, value);
	}

	template<class T, class U>
	[[nodiscard]] std::optional<T> FromV8(v8::Isolate& isolate, const v8::Local<U>& value)
	{
		return Type<T>::FromV8(isolate, value);
	}

	template<class T, class U>
	[[nodiscard]] std::optional<T> FromV8(v8::Isolate& isolate, const v8::MaybeLocal<U>& value)
	{
		v8::Local<v8::Value> possibleValue;
		return value.ToLocal(&possibleValue) ? Type<T>::FromV8(isolate, possibleValue) : std::optional<T>{};
	}

	template<class T>
	[[nodiscard]] std::optional<T> FromV8(const v8::Maybe<T>& value)
	{
		T extracted;
		return value.To(&extracted) ? extracted : std::optional<T>{};
	}

	template<class T, class U>
	[[nodiscard]] T FromV8Required(v8::Isolate& isolate, const v8::Local<U>& value)
	{
		const auto from = FromV8<T>(isolate, value);
		if (!from)
			throw Error("Required value was not found.");
		return *from;
	}

	template<class T, class U>
	[[nodiscard]] T FromV8Required(v8::Isolate& isolate, const v8::MaybeLocal<U>& value)
	{
		const auto from = FromV8<T>(isolate, value);
		if (!from)
			throw Error("Required value was not found.");
		return *from;
	}
}